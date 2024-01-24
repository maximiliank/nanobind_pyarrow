import numpy as np
import pyarrow as pa
import pytest
import test_chunked_array_ext as t


@pytest.mark.parametrize(
    "dtype",
    [
        np.float16,
        np.float32,
        np.float64,
        np.int8,
        np.int16,
        np.int32,
        np.int64,
        np.uint8,
        np.uint16,
        np.uint32,
        np.uint64,
    ],
)
def test_chunked_numeric_array(dtype):
    np_arr = np.array([1.0, 2.0, 3.0], dtype=dtype())
    arr = pa.array(np_arr)
    chunked_arr = pa.chunked_array(arr)
    assert t.test_chunked_array(chunked_arr).equals(chunked_arr)
