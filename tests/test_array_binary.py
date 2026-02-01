import importlib

import pyarrow as pa
import pytest


@pytest.fixture(
    params=["test_array_binary_ext", "test_array_binary_ext_capi"],
    ids=["standard", "capi"],
)
def t(request):
    return importlib.import_module(request.param)


@pytest.mark.parametrize(
    "dtype", [pa.string, pa.large_string, pa.binary_view, pa.string_view]
)
def test_string_array(dtype, t):
    arr = pa.array(["foo", "bar"] * 50, type=dtype())
    type_str = str(arr.type).split("[")[0]
    print(type_str)
    func = getattr(t, f"test_{type_str}_array")
    assert func(arr).equals(arr)
