import datetime
import decimal

import numpy as np
import pyarrow as pa
import pytest
import test_scalar_ext as t


@pytest.mark.parametrize(
    ["value", "ty", "callback"],
    [
        (False, None, "boolean"),
        (True, None, "boolean"),
        (1, None, "int64"),
        (-1, None, "int64"),
        (1, pa.int8(), "int8"),
        (1, pa.uint8(), "uint8"),
        (1, pa.int16(), "int16"),
        (1, pa.uint16(), "uint16"),
        (1, pa.int32(), "int32"),
        (1, pa.uint32(), "uint32"),
        (1, pa.int64(), "int64"),
        (1, pa.uint64(), "uint64"),
        (1.0, None, "double"),
        (np.float16(1.0), pa.float16(), "halffloat"),
        (1.0, pa.float32(), "float"),
        (decimal.Decimal("1.123"), None, "decimal128"),
        (decimal.Decimal("1.1234567890123456789012345678901234567890"), None, "decimal256"),
        ("string", None, "string"),
        (b"bytes", None, "binary"),
        ("largestring", pa.large_string(), "largestring"),
        (b"largebytes", pa.large_binary(), "largebinary"),
        (b"abc", pa.binary(3), "fixedsizebinary"),
        ([1, 2, 3], None, "list"),
        ([1, 2, 3, 4], pa.large_list(pa.int8()), "largelist"),
        ([1, 2, 3, 4, 5], pa.list_(pa.int8(), 5), "fixedsizelist"),
        (datetime.date.today(), None, "date32"),
        (datetime.date.today(), pa.date64(), "date64"),
        (datetime.datetime.now(), None, "timestamp"),
        (datetime.datetime.now().time().replace(microsecond=0), pa.time32("s"), "time32"),
        (datetime.datetime.now().time(), None, "time64"),
        (datetime.timedelta(days=1), None, "duration"),
        (pa.MonthDayNano([1, -1, -10100]), None, "monthdaynanointerval"),
        ({"a": 1, "b": [1, 2]}, None, "struct"),
        ([("a", 1), ("b", 2)], pa.map_(pa.string(), pa.int8()), "map"),
        (memoryview(b"abc"), pa.binary_view(), "binaryview"),
        ("abc", pa.string_view(), "stringview"),
    ],
)
def test_scalar(value, ty, callback):
    s = pa.scalar(value, type=ty)
    s.validate()
    s.validate(full=True)
    func = getattr(t, f"test_{callback}_scalar")
    assert func(s).equals(s)
