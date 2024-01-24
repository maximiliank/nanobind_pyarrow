import pyarrow as pa
import test_buffer_ext as t


def test_buffer():
    buffer = pa.allocate_buffer(10)
    assert t.test_buffer(buffer).equals(buffer)
    buffer = pa.allocate_buffer(10, resizable=True)
    assert t.test_mutable_buffer(buffer).equals(buffer)
    assert t.test_resizable_buffer(buffer).equals(buffer)
