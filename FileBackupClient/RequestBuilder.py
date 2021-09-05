import os
import struct
from ProtocolCommon import *


def save_file_payload_generator(file_name):
    """
    Build payload generator for save file operation
    :param file_name: name of the file to save
    """
    # Yield file size
    remaining_bytes_to_read = os.path.getsize(file_name)
    yield struct.pack(PAYLOAD_SIZE_FMT, remaining_bytes_to_read)

    # Yield file content
    with open(file_name, "rb") as f:
        while remaining_bytes_to_read > 0:
            bytes_to_read = min(CHUNK_SIZE, remaining_bytes_to_read)
            yield f.read(bytes_to_read)
            remaining_bytes_to_read -= bytes_to_read


def _create_request_header(user_id, version, op, filename=None):
    """
    Create request header buffer
    :param user_id: request user identifier
    :param version: request protocol version
    :param op: request operation
    :param filename: request file name
    :return: request header buffer
    """
    fmt = REQUEST_HEADER_NO_FILENAME_FMT
    args = [user_id, version, op]

    if filename is not None:
        # Request has file name field, add it to header
        fmt = REQUEST_HEADER_WITH_FILENAME_FMT.format(len(filename))
        args += [len(filename), str.encode(filename)]

    return struct.pack(fmt, *args)


def build_save_file_request_header(user_id, version, filename):
    """
    Build save file operation request header
    :param user_id: user identifier
    :param version: protocol version
    :param filename: name of the file to save
    :return: buffer of the request header
    """
    return _create_request_header(user_id, version, OPERATIONS['SendFile'], filename)


def build_delete_file_request(user_id, version, filename):
    """
    Build delete file operation request
    :param user_id: user identifier
    :param version: protocol version
    :param filename: name of the file to delete
    :return: buffer of the request
    """
    return _create_request_header(user_id, version, OPERATIONS['DeleteFile'], filename)


def build_return_file_request(user_id, version, filename):
    """
    Build return file operation request
    :param user_id: user identifier
    :param version: protocol version
    :param filename: name of the file to return
    :return: buffer of the request
    """
    return _create_request_header(user_id, version, OPERATIONS['ReturnFile'], filename)


def build_file_list_request(user_id, version):
    """
    Build get file list operation request
    :param user_id: user identifier
    :param version: protocol version
    :return: buffer of the request
    """
    return _create_request_header(user_id, version, OPERATIONS['GetFileList'])
