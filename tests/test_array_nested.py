import pyarrow as pa
import test_array_nested_ext as t


def test_list_array():
    values = pa.array([1, 2, 3, 4])
    offsets = pa.array([0, 2, 4])
    arr = pa.ListArray.from_arrays(offsets, values)
    assert t.test_list_array(arr).equals(arr)
