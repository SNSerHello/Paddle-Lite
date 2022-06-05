#include <assert.h>
#include <hiredis/hiredis.h>
#include <iostream>
#include <string.h>

int main(void) {
  redisContext *conn = redisConnect("127.0.0.1", 6379);
  std::cout << "redis connection: " << std::hex << conn << std::endl;
  redisReply *r = (redisReply *)redisCommand(conn, "hget cv cat");
  std::cout << "redis reply: " << std::hex << r << std::endl;
  if (r->type == REDIS_REPLY_STRING) {
    if (r->len > 0) {
      const int elements = r->len / 4;
      float *data = new float[r->len / 4];
      memcpy(data, r->str, r->len);
      std::cout.precision(8);
      std::cout << "B layer" << std::endl;
      // [0.1254902 , 0.1254902 , 0.1254902 , 0.1254902 , 0.1254902 ,
      //  0.12156863, 0.12941177, 0.13725491, 0.13725491, 0.13725491,
      //  0.14117648, 0.14509805, 0.14901961, 0.15294118, 0.15294118]
      for (int i = 0; i < 15; i++) {
        std::cout << data[i] << ", ";
        if ((i + 1) % 5 == 0)
          std::cout << std::endl;
      }
      std::cout << std::endl;
      std::cout << "G layer" << std::endl;
      // [0.26666668, 0.26666668, 0.26666668, 0.2627451 , 0.26666668,
      //  0.2627451 , 0.2627451 , 0.25882354, 0.25882354, 0.25882354,
      //  0.2627451 , 0.26666668, 0.27058825, 0.27450982, 0.27450982]
      for (int i = 0; i < 15; i++) {
        std::cout << data[1 * 224 * 224 + i] << ", ";
        if ((i + 1) % 5 == 0)
          std::cout << std::endl;
      }
      std::cout << std::endl;
      std::cout << "R layer" << std::endl;
      // [0.12156863, 0.11764706, 0.11764706, 0.11372549, 0.11764706,
      //  0.11372549, 0.11764706, 0.10980392, 0.09803922, 0.09803922,
      //  0.10196079, 0.10196079, 0.10196079, 0.10196079, 0.10196079]
      for (int i = 0; i < 15; i++) {
        std::cout << data[2 * 224 * 224 + i] << ", ";
        if ((i + 1) % 5 == 0)
          std::cout << std::endl;
      }
      std::cout << std::endl;
      delete[] data;
    } else {
      std::cout << "unknow len: " << r->len << std::endl;
    }
  } else {
    switch (r->type) {
    case REDIS_REPLY_STATUS:
      std::cout << "REDIS_REPLY_STATUS: " << REDIS_REPLY_STATUS << std::endl;
      break;
    case REDIS_REPLY_ERROR:
      std::cout << "REDIS_REPLY_ERRORL " << REDIS_REPLY_ERROR << std::endl;
      break;
    case REDIS_REPLY_INTEGER:
      std::cout << "REDIS_REPLY_INTEGER: " << REDIS_REPLY_INTEGER << std::endl;
      break;
    case REDIS_REPLY_NIL:
      std::cout << "REDIS_REPLY_NIL: " << REDIS_REPLY_NIL << std::endl;
      break;
    case REDIS_REPLY_STRING:
      std::cout << "REDIS_REPLY_STRING: " << REDIS_REPLY_STRING << std::endl;
      break;
    case REDIS_REPLY_ARRAY:
      std::cout << "REDIS_REPLY_ARRAY: " << REDIS_REPLY_ARRAY << std::endl;
      break;
    default:
      std::cout << "unknown type: " << r->type << std::endl;
    }
  }
  freeReplyObject(r);
  redisFree(conn);
  return 0;
}