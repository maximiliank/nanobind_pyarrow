import pyarrow as pa
import pytest
import test_array_binary_ext as t


@pytest.mark.parametrize("dtype", [pa.string, pa.large_string, pa.binary_view, pa.string_view])
def test_string_array(dtype):
    arr = pa.array(["foo", "bar"] * 50, type=dtype())
    type_str = str(arr.type).split("[")[0]
    print(type_str)
    func = getattr(t, f"test_{type_str}_array")
    assert func(arr).equals(arr)
