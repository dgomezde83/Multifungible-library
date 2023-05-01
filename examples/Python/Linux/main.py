import ctypes
import os

# Load the dynamic library
mylibrary = ctypes.cdll.LoadLibrary("../../../lib/libMultifungible.so")

# Define the struct returned by the function
class MyStruct(ctypes.Structure):
    _fields_ = [("my_int", ctypes.c_int),
                ("my_string", ctypes.c_char_p)]

# Define the function signature
my_function = mylibrary.createWallet
my_function.restype = MyStruct

# Call the function
result = my_function(b"myjsn.json",b"1234")

# Access the struct fields
print(result.my_int)
print(result.my_string.decode())