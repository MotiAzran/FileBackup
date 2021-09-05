import socket
import struct
import RequestBuilder
from common import *
from ProtocolCommon import *


class ProtocolSocket(object):
    """
    Send request and receive response by the protocol
    """

    def __init__(self, host, user_id, version):
        self._sock = None
        self._host = host
        self._id = user_id
        self._version = version

        self._with_payload_statuses = (210, 211)
        self._success_statuses = self._with_payload_statuses + (212,)
        self._error_exceptions = {1001: self._file_not_found_error, 1002: NoFiles,
                                  1003: GeneralServerError}

    def __enter__(self):
        self.connect()

        return self

    def __exit__(self, exception_type, exception_value, exception_traceback):
        self.close()
        if exception_type is not None:
            print(f"Error: {exception_value}")

    def connect(self):
        # Connect to server
        self._sock = socket.socket()
        self._sock.connect(self._host)

    def close(self):
        self._sock.close()

    def send_save_file_request(self, filename, payload_gen):
        """
        Send save file request to server
        :param filename: file name to send for backup
        :param payload_gen: generator of the payload (file content)
        """
        # Build and send request
        request_header = RequestBuilder.build_save_file_request_header(self._id, self._version, filename)
        self._sock.sendall(request_header)

        # Send file content
        for content in payload_gen:
            self._sock.sendall(content)

    def send_return_file_request(self, filename):
        """
        Send server request to retrieve file
        :param filename: file name to return
        """
        request = RequestBuilder.build_return_file_request(self._id, self._version, filename)
        self._sock.sendall(request)

    def send_delete_file_request(self, filename):
        """
        Send server request to delete file
        :param filename: file name to delete
        """
        request = RequestBuilder.build_delete_file_request(self._id, self._version, filename)
        self._sock.sendall(request)

    def send_file_list_request(self):
        """
        Send server request to retrieve user files list
        """
        request = RequestBuilder.build_file_list_request(self._id, self._version)
        self._sock.sendall(request)

    def _should_return_payload(self, status):
        """
        Check if status has payload
        :param status: status to check
        :return: True if the status has payload by the protocol,
                else False
        """
        return status in self._with_payload_statuses

    def _receive_response_header(self):
        """
        Receive response header from the server
        :return: The received response header fields
        """
        header = self._sock.recv(struct.calcsize(RESPONSE_HEADER_NO_FILENAME_FMT))
        version, status = struct.unpack(RESPONSE_HEADER_NO_FILENAME_FMT, header)

        # Check status
        if status not in self._success_statuses:
            raise self._error_exceptions.get(status, UnknownServerError)()

        return version, status

    def _receive_sized_buffer(self, size_fmt):
        """
        Generator that receive buffer that sent with his size
        in chunks
        :param size_fmt: the format of the size that should
                        received from the server
        :return: The received buffer
        """
        # Receive buffer size
        data = self._sock.recv(struct.calcsize(size_fmt))
        size = struct.unpack(size_fmt, data)[0]

        # Yield buffer in chunks
        remaining_data_size = size
        while remaining_data_size > 0:
            bytes_to_read = min(remaining_data_size, CHUNK_SIZE)
            data = self._sock.recv(bytes_to_read)
            yield data
            remaining_data_size -= bytes_to_read

    def _receive_filename(self):
        """
        Receive file name by the protocol
        :return: Received file name
        """
        filename = ''
        for data in self._receive_sized_buffer(FILENAME_SIZE_FMT):
            filename += data.decode('utf-8')

        return filename

    def _receive_payload(self):
        """
        Generator to receive payload by the protocol in chunks
        """
        yield from self._receive_sized_buffer(PAYLOAD_SIZE_FMT)

    def _file_not_found_error(self):
        return FileNotFound(self._receive_filename())

    def receive_response(self):
        """
        Receive response from the server
        :return: tuple of response status and file name
                and payload generator if exists
        """
        # Receive response header
        version, status = self._receive_response_header()
        filename = self._receive_filename()

        if not self._should_return_payload(status):
            # The response has no payload
            return status, filename

        payload_gen = self._receive_payload()

        return status, filename, payload_gen
