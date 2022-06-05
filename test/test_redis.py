import redis

r = redis.Redis()
if r.hget("cv", "cat") is None:
    import numpy as np
    from PIL import Image

    img = (
        (
            np.asarray(
                Image.open("cat.jpg").crop((200, 0, 1400, 1200)).resize((224, 224))
            )
            / 255.0
        )
        .astype(np.float32)
        .transpose((2, 0, 1))[np.newaxis, ...]
    )
    if r.hset("cv", "cat", img.tobytes()) == 0:
        print("Failed to store cat data into redis server")
