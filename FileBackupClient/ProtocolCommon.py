REQUEST_HEADER_NO_FILENAME_FMT = "<LBB"
REQUEST_HEADER_WITH_FILENAME_FMT = "<LBBH{}s"

FILENAME_SIZE_FMT = "<H"
FILENAME_FMT = FILENAME_SIZE_FMT + "{}s"

RESPONSE_HEADER_NO_FILENAME_FMT = "<BH"

PAYLOAD_SIZE_FMT = "<L"

CHUNK_SIZE = 65 * 1024

# Server operations
OPERATIONS = {'SendFile': 100, 'ReturnFile': 200, 'DeleteFile': 201, 'GetFileList': 202}

# Message to print when receiving the success status
SUCCESS_STATUS_MESSAGE = {
    210: "210: File {} returned",
    211: "211: File list received",
    212: "212: File {} {} successfully",
}
