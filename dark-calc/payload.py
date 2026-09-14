import sys
# TODO: modify the payload to execute malware
payload = b'\x10\x10\x10\x10\x10\x10\x10\x10'\
        + b'\x10\x10\x10\x10\x10\x10\x10\x10'
sys.stdout.buffer.write(payload)