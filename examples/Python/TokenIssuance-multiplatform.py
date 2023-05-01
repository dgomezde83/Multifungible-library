import ctypes
import os
import platform

# Load the dynamic library
mylibrary = None
if platform.system() == "Windows":
    mylibrary = ctypes.cdll.LoadLibrary("../../lib/libMultifungible.dll")
elif platform.system() == "Linux":
    mylibrary = ctypes.cdll.LoadLibrary("../../lib/libMultifungible.so")
else:
    # Raise an exception for other platforms
    raise Exception("Unsupported platform: {}".format(platform.system()))

# Define the struct returned by the function
class MyStruct(ctypes.Structure):
    _fields_ = [("my_int", ctypes.c_int),
                ("my_string", ctypes.c_char_p)]

# Create a new wallet with the name myPEMFile.json and load it with > 0.05 EGLD (you have to do this on your own)

if os.path.isfile("./myPEMFile.json"):
  load_wallet = mylibrary.loadWallet
  load_wallet.restype = MyStruct
  wallet = load_wallet(b"./myPEMFile.json",b"1234")
  print(wallet.my_string.decode())
else:
  create_wallet = mylibrary.createWallet
  create_wallet.restype = MyStruct
  wallet = create_wallet(b"./myPEMFile.json",b"1234")
  print(wallet.my_string.decode())

# Define the function signature for issuing a collection
issue_SFT_Collection = mylibrary.issueSFTCollection
issue_SFT_Collection.restype = MyStruct

# Call the function
c_true = ctypes.c_bool(True)
collectionID = issue_SFT_Collection(b"./myPEMFile.json", # PEM file path (needs to be created and loaded with 0.05 EGLD)
                               b"1234",  # Password
                               b"Test",  # Collection name
                               b"TST" ,
				c_true,
				c_true,
				c_true,
				c_true,
				c_true,
				c_true,
				c_true) # Collection ticker
if collectionID.my_int == 1:
  raise Exception("Error: {}".format(collectionID.my_string.decode()))
           
# Define the function signature for issuing a token
issue_SFT_Token = mylibrary.issueSemiFungibleToken
issue_SFT_Token.restype = MyStruct

# Call the function
tokenID = issue_SFT_Token(b"./myPEMFile.json", # PEM file path (needs to be created and loaded with 0.05 EGLD)
                               b"1234",                  # Password
                               collectionID.encode("utf-8"),  # collection name
                               b"tokenTest",             # Name of the token
                               b"10",                    # quantity
                               b"8500",                  # Royalties (85.00%)
                               b"metadata:ipfsCID/fileName.json;tags:tag1,tag2,tag3", # metadata 
                               b"") # URL

if tokenID.my_int == 1:
  raise Exception("Error: {}".format(tokenID.my_string.decode()))
  
# Print our token name
print(tokenID.my_string.decode())
