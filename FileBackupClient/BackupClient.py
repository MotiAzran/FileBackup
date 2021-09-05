from ProtocolSocket import ProtocolSocket
from ProtocolCommon import SUCCESS_STATUS_MESSAGE
from common import *


class BackupClient(object):
    """
    Represent a client for backup server
    """

    def __init__(self, host, user_id, version):
        # Validate params
        if user_id > MAX_DWORD:
            raise ValueError("Invalid user ID")

        if version > MAX_BYTE:
            raise ValueError("Invalid version")

        self._host = host
        self._user_id = user_id
        self._version = version

    def _safe_receive_response(self, sock):
        """
        Receive response safely (with exceptions handling)
        :param sock: socket to receive the response from
        :return: server response
        """
        try:
            return sock.receive_response()
        except NonCriticalException as e:
            print("Server error: {}".format(e))
            return
        except CriticalException as e:
            print("Error: {}".format(e))
            raise e
        except Exception as e:
            print("Error: {}".format(e))
            raise e

    def _create_socket(self):
        return ProtocolSocket(self._host, self._user_id, self._version)

    def send_file_for_backup(self, filename, content_gen):
        """
        Send file to server for backup
        :param filename: name of the file to send
        :param content_gen: generator that retrieve the file content
        """
        with self._create_socket() as sock:
            sock.send_save_file_request(filename, content_gen)

            response = self._safe_receive_response(sock)
            if response is None:
                return

            status, server_filename = response

            if server_filename != filename:
                raise ValueError("Got different server file name {} != {}".format(server_filename, filename))

            print(SUCCESS_STATUS_MESSAGE[status].format(server_filename, "saved"))

    def request_file(self, filename):
        """
        Request file from the server
        :param filename: name of the file to retrieve
        """
        with self._create_socket() as sock:
            sock.send_return_file_request(filename)

            response = self._safe_receive_response(sock)
            if response is None:
                return

            status, server_filename, payload_gen = response
            if server_filename != filename:
                raise ValueError("Got different server file name {} != {}".format(server_filename, filename))

            print(SUCCESS_STATUS_MESSAGE[status].format(server_filename))

            # Write received content to file
            with open(server_filename, 'wb') as f:
                for content in payload_gen:
                    f.write(content)

    def delete_file(self, filename):
        """
        Delete file on the server
        :param filename: name of the file to delete
        """
        with self._create_socket() as sock:
            sock.send_delete_file_request(filename)

            response = self._safe_receive_response(sock)
            if response is None:
                return

            status, server_filename = response
            if server_filename != filename:
                raise ValueError("Got different server file name {} != {}".format(server_filename, filename))

            print(SUCCESS_STATUS_MESSAGE[status].format(server_filename, "deleted"))

    def _print_file_list(self, file_list_gen):
        """
        Get in chunks file names each in seperate line
        and print those names to screen
        """
        remaining = str()
        for data in file_list_gen:
            # Convert data to string
            data = remaining + data.decode('utf-8')

            # Save incomplete file name for next iteration
            remaining_end_index = data.rfind('\n')
            if remaining_end_index == -1:
                remaining = data
                continue

            remaining = data[remaining_end_index + 1:]

            # Remove remaining
            data = data[:remaining_end_index]
            for line in data.splitlines(keepends=False):
                print('\t- {}'.format(line))

    def get_file_list(self):
        """
        Get user files list from the server
        """
        with self._create_socket() as sock:
            sock.send_file_list_request()

            response = self._safe_receive_response(sock)
            if response is None:
                return

            status, filename, payload_gen = response
            print(SUCCESS_STATUS_MESSAGE[status])

            self._print_file_list(payload_gen)
