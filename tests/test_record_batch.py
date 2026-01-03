import importlib

import pyarrow as pa
import pytest


@pytest.fixture(
    params=["test_record_batch_ext", "test_record_batch_ext_capi"],
    ids=["standard", "capi"],
)
def t(request):
    return importlib.import_module(request.param)


def test_record_batch(t):
    n_legs = pa.array([2, 2, 4, 4, 5, 100])
    animals = pa.array(
        ["Flamingo", "Parrot", "Dog", "Horse", "Brittle stars", "Centipede"]
    )
    names = ["n_legs", "animals"]
    data = pa.RecordBatch.from_arrays([n_legs, animals], names=names)
    assert t.test_record_batch(data).equals(data)
