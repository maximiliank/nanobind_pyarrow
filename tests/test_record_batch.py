import pyarrow as pa
import test_record_batch_ext as t


def test_record_batch():
    n_legs = pa.array([2, 2, 4, 4, 5, 100])
    animals = pa.array(["Flamingo", "Parrot", "Dog", "Horse", "Brittle stars", "Centipede"])
    names = ["n_legs", "animals"]
    data = pa.RecordBatch.from_arrays([n_legs, animals], names=names)
    assert t.test_record_batch(data).equals(data)
