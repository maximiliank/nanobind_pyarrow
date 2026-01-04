import importlib

import pyarrow as pa
import pytest


@pytest.fixture(
    params=["test_type_ext", "test_type_ext_capi"],
    ids=["standard", "capi"],
)
def t(request):
    return importlib.import_module(request.param)


def test_field(t):
    f = pa.field("test", pa.int32())
    f_copy = t.test_field(f)
    assert f_copy.equals(f)


def test_schema(t):
    schema = pa.schema([("some_int", pa.int32()), ("some_string", pa.string())])
    schema_copy = t.test_schema(schema)
    assert schema_copy.equals(schema)
