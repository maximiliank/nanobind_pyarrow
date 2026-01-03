import importlib

import pyarrow as pa
import pytest


@pytest.fixture(
    params=["test_array_nested_ext", "test_array_nested_ext_capi"],
    ids=["standard", "capi"],
)
def t(request):
    return importlib.import_module(request.param)


def test_list_array(t):
    values = pa.array([1, 2, 3, 4])
    offsets = pa.array([0, 2, 4])
    arr = pa.ListArray.from_arrays(offsets, values)
    assert t.test_list_array(arr).equals(arr)
