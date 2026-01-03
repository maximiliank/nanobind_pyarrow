import importlib

import pyarrow as pa
import pytest


@pytest.fixture(
    params=["test_table_ext", "test_table_ext_capi"],
    ids=["standard", "capi"],
)
def t(request):
    return importlib.import_module(request.param)


def table_data() -> pa.Table:
    n_legs = pa.array([2, 2, 4, 4, 5, 100])
    animals = pa.array(
        ["Flamingo", "Parrot", "Dog", "Horse", "Brittle stars", "Centipede"]
    )
    names = ["n_legs", "animals"]
    data = pa.Table.from_arrays([n_legs, animals], names=names)
    return data


def table_with_chunks() -> pa.Table:
    data = table_data()
    batches = data.to_batches()
    table = pa.Table.from_batches([batches[0], batches[0]])
    assert len(table.to_batches()) > 1
    return table


def test_table(t):
    data = table_data()
    assert t.test_table(data).equals(data)


def test_table_with_chunks(t):
    data = table_with_chunks()
    assert t.test_table(data).equals(data)
