from Crypto.PublicKey import ECC
from Crypto.Signature import DSS
from Crypto.Hash import SHA3_256
import binascii

# Large message embedded directly in the script
message = """
{"nonce":5,"value":"50000000000000000","receiver":"erd1qqqqqqqqqqqqqqqpqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqzllls8a5w6u","sender":"erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th","gasPrice":99999,"gasLimit":60000000,"data":"aXNzdWVANDE2YzY5NjM2NUA0MTRjNDNAMDNlOEAwYUA2MzYxNmU0NjcyNjU2NTdhNjVANjY2MTZjNzM2NUA2MzYxNmU1NzY5NzA2NUA2NjYxNmM3MzY1QDYzNjE2ZTUwNjE3NTczNjVANjY2MTZjNzM2NUA2MzYxNmU0MzY4NjE2ZTY3NjU0Zjc3NmU2NTcyQDc0NzI3NTY1QDYzNjE2ZTU1NzA2NzcyNjE2NDY1QDY2NjE2YzczNjVANjM2MTZlNDE2NDY0NTM3MDY1NjM2OTYxNmM1MjZmNmM2NTczQDc0NzI3NTY1","chainID":"1","version":6,"options":7}
"""

# Load the EC private key from the PEM file
# Replace 'alicePem.pem' with the path to your PEM file
with open('alicePem.pem', 'r') as f:
    private_key = ECC.import_key(f.read())

# Compute the SHA3-256 hash of the message
h = SHA3_256.new(message.encode('utf-8'))

# Create a signer object and sign the hash
signer = DSS.new(private_key, 'fips-186-3')
signature = signer.sign(h)

# Convert the signature to hexadecimal format
hex_signature = binascii.hexlify(signature).decode()

# Output the hexadecimal signature
print(hex_signature)

