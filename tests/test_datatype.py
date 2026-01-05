import importlib
import re

import pyarrow as pa
import pytest


@pytest.fixture(
    params=["test_datatype_ext", "test_datatype_ext_capi"],
    ids=["standard", "capi"],
)
def t(request):
    return importlib.import_module(request.param)


data_types = [
    pa.null(),
    pa.bool_(),
    pa.int8(),
    pa.int16(),
    pa.int32(),
    pa.int64(),
    pa.uint8(),
    pa.uint16(),
    pa.uint32(),
    pa.uint64(),
    pa.float16(),
    pa.float32(),
    pa.float64(),
    pa.time32("s"),
    pa.time64("us"),
    pa.timestamp("us"),
    pa.date32(),
    pa.date64(),
    pa.duration("s"),
    pa.month_day_nano_interval(),
    pa.binary(),
    pa.binary(10),
    pa.string(),
    pa.large_string(),
    pa.large_binary(),
    pa.decimal128(19, 4),
    pa.decimal256(76, 38),
    pa.list_(pa.int32()),
    pa.list_(pa.int32(), 2),
    pa.large_list(pa.uint16()),
    pa.map_(pa.string(), pa.int32()),
    pa.struct(
        [
            pa.field("a", pa.int32()),
            pa.field("b", pa.int8()),
            pa.field("c", pa.string()),
        ]
    ),
    pa.union(
        [pa.field("a", pa.binary(10)), pa.field("b", pa.string())],
        mode=pa.lib.UnionMode_DENSE,
    ),
    pa.union(
        [pa.field("a", pa.binary(10)), pa.field("b", pa.string())],
        mode=pa.lib.UnionMode_SPARSE,
    ),
    pa.dictionary(pa.int32(), pa.string()),
    pa.run_end_encoded(pa.int64(), pa.uint8()),
    pa.string_view(),
    pa.binary_view(),
]
data_type_ids = [str(dt) for dt in data_types]


@pytest.mark.parametrize("data_type", data_types, ids=data_type_ids)
def test_data_types(data_type, t):
    def resolve_callback():
        callback_name = type(data_type).__name__.lower()
        if callback_name == "datatype":
            callback_name = re.sub(r"\[.*?\]", "", str(data_type).split("(")[0])
        else:
            callback_name = callback_name.replace("type", "")
        return getattr(t, f"test_{callback_name}_type")

    func = resolve_callback()
    assert func(data_type).equals(data_type)
    assert t.test_data_type(data_type).equals(data_type)
