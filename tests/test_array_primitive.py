import importlib

import numpy as np
import pyarrow as pa
import pytest


@pytest.fixture(
    params=["test_array_primitive_ext", "test_array_primitive_ext_capi"],
    ids=["standard", "capi"],
)
def t(request):
    return importlib.import_module(request.param)


@pytest.mark.parametrize(
    "data,func_name",
    [
        (pa.array([1, 2, 3]), "test_array"),
        (pa.nulls(10), "test_null_array"),
        (pa.array([True, False, False]), "test_boolean_array"),
    ],
)
def test_base_arrays(data, func_name, t):
    func = getattr(t, func_name)
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
def test_numeric_array(dtype, t):
    np_arr = np.array([1.0, 2.0, 3.0], dtype=dtype())
    arr = pa.array(np_arr)
    func = getattr(t, f"test_{str(arr.type)}_array")
    assert func(arr).equals(arr)


@pytest.mark.parametrize(
    "dtype",
    [pa.time32("s"), pa.time32("ms"), pa.time64("us"), pa.time64("ns")],
)
def test_time_array(dtype, t):
    arr = pa.array([1, 2, 3], type=dtype)
    func = getattr(t, f"test_{str(arr.type).split('[')[0]}_array")
    assert func(arr).equals(arr)


@pytest.mark.parametrize(
    "dtype",
    [pa.timestamp("s"), pa.timestamp("ms"), pa.timestamp("us"), pa.timestamp("ns")],
)
def test_timestamp_array(dtype, t):
    arr = pa.array([1, 2, 3], type=dtype)
    func = getattr(t, f"test_{str(arr.type).split('[')[0]}_array")
    assert func(arr).equals(arr)
