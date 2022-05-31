import numpy as np
from PIL import Image
import onnx
import onnxruntime as ort

img = (
    (
        np.asarray(Image.open("cat.jpg").crop((200, 0, 1400, 1200)).resize((224, 224)))
        / 255.0
    )
    .astype(np.float32)[..., ::-1]
    .transpose((2, 0, 1))[np.newaxis, ...]
)

sess = ort.InferenceSession("mobilenet_v1.onnx", providers=["CPUExecutionProvider"])
input_name = sess.get_inputs()[0].name
output_name = sess.get_outputs()[0].name
print(
    """
mobilenet v1
    input name  : {}
    output name : {}
        """.format(
        input_name, output_name
    )
)
out = sess.run(output_names=[output_name], input_feed={input_name: img})[0]

with open("synset.txt", "r", encoding="utf-8") as f:
    labels = np.array([line.strip() for line in f if line.strip()])

o_index = out.argsort(axis=-1)[..., -5:][..., ::-1]
print(
    """
Top5 index  : {}
Top5 probs  : {}
Top5 labels : {}
""".format(
        o_index.ravel(),
        out[..., o_index].ravel(),
        "\n{}".format(" " * 14).join(labels[o_index].ravel()),
    )
)
