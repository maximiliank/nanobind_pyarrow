import pyarrow as pa
import test_type_ext as t


def test_field():
    f = pa.field("test", pa.int32())
    f_copy = t.test_field(f)
    assert f_copy.equals(f)


def test_schema():
    schema = pa.schema([("some_int", pa.int32()), ("some_string", pa.string())])
    schema_copy = t.test_schema(schema)
    assert schema_copy.equals(schema)
