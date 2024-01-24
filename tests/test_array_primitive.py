import numpy as np
import pyarrow as pa
import pytest
import test_array_primitive_ext as t


@pytest.mark.parametrize(
    "data,func",
    [
        (pa.array([1, 2, 3]), t.test_array),
        (pa.nulls(10), t.test_null_array),
        (pa.array([True, False, False]), t.test_boolean_array),
    ],
)
def test_base_arrays(data, func):
    assert func(data).equals(data)


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
def test_numeric_array(dtype):
    np_arr = np.array([1.0, 2.0, 3.0], dtype=dtype())
    arr = pa.array(np_arr)
    func = getattr(t, f"test_{str(arr.type)}_array")
    assert func(arr).equals(arr)
