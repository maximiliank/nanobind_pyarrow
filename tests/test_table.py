import pyarrow as pa
import test_table_ext as t


def test_table():
    n_legs = pa.array([2, 2, 4, 4, 5, 100])
    animals = pa.array(["Flamingo", "Parrot", "Dog", "Horse", "Brittle stars", "Centipede"])
    names = ["n_legs", "animals"]
    data = pa.Table.from_arrays([n_legs, animals], names=names)
    assert t.test_table(data).equals(data)
