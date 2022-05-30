from paddlelite import lite

config = lite.MobileConfig()
config.set_model_from_file("mobilenet_v1_opencl.nb")
predictor = lite.create_paddle_predictor(config)

import numpy as np
from PIL import Image

img = (
    (
        np.asarray(Image.open("cat.jpg").crop((200, 0, 1400, 1200)).resize((224, 224)))
        / 255.0
    )
    .astype(np.float32)
    .transpose((2, 0, 1))[np.newaxis, ...]
)

input_tensor = predictor.get_input(0)
input_tensor.from_numpy(img)
predictor.run()
out = predictor.get_output(0).numpy()

with open("synset.txt", "r", encoding="utf-8") as f:
    labels = np.array([line.strip() for line in f if line.strip()])

# Mobilenet v1 has included softmax
# from scipy.special import softmax
# out = softmax(out, axis=-1)

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
