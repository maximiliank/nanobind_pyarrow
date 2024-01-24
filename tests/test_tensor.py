import numpy as np
import pyarrow as pa
import test_tensor_ext as t


def test_tensor():
    arr = np.array([[2, 2, 4], [4, 5, 100]], np.int32)
    tensor = pa.Tensor.from_numpy(arr, dim_names=["dim1", "dim2"])
    assert t.test_tensor(tensor).equals(tensor)
