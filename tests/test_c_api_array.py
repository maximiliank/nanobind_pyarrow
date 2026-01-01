import numpy as np
import pyarrow as pa
import test_c_api_array_ext as t


def test_numeric_array():
    np_arr = np.array([1.0, 2.0, 3.0], dtype=np.float64)
    arr = pa.array(np_arr)
    func = getattr(t, f"test_{str(arr.type)}_array")
    assert func(arr).equals(arr)


if __name__ == "__main__":
    test_numeric_array()
