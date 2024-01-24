import pyarrow as pa
import pytest
import test_array_binary_ext as t


@pytest.mark.parametrize("dtype", [pa.string, pa.large_string])
def test_string_array(dtype):
    arr = pa.array(["foo", "bar"] * 50, type=dtype())
    func = getattr(t, f"test_{str(arr.type)}_array")
    assert func(arr).equals(arr)
