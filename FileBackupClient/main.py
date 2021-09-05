import os
import random

from RequestBuilder import save_file_payload_generator
from BackupClient import BackupClient
from NetworkUtils import validate_ip, validate_port

VERSION = 1
MAX_USER_ID = 0xffffffff


def get_user_id():
    return random.randint(0, MAX_USER_ID)


def get_server_info():
    """
    Return server IP address and port number
    :return: tuple of IP and port
    """
    with open('server.info') as f:
        data = f.read()

    if data.count(':') != 1:
        raise Exception("server.info format is invalid")

    ip, port = data.split(':')

    if not validate_ip(ip) or not validate_port(port):
        raise ValueError('Invalid server info')

    return ip, int(port)


def get_backup_files_list():
    """
    Get list of files to backup
    """
    with open('backup.info') as f:
        files = f.read().splitlines(keepends=False)

    # Empty file
    if not files:
        raise Exception("No files")

    # Check that there is no paths
    if any((os.path.basename(f) != f for f in files)):
        raise Exception("Write base name only")

    # Some files doesn't exists
    if any((not os.path.exists(f) for f in files)):
        raise Exception("Some files doesn't exists")

    # Check all files are actually files
    if any((not os.path.isfile(f) for f in files)):
        raise Exception("Some files aren't actually a file")

    return files


def save_files(files, client):
    """
    Backup files
    :param files: file names list to backup
    :param client: BackupClient instance
    """
    for file_name in files:
        if not os.path.exists(file_name):
            raise ValueError("File {} not exists".format(file_name))

        client.send_file_for_backup(file_name, save_file_payload_generator(file_name))


def main():
    # Initialize info
    user_id = get_user_id()
    host = get_server_info()
    files = get_backup_files_list()

    # Create client
    client = BackupClient(host, user_id, VERSION)

    client.get_file_list()

    save_files(files, client)
    client.get_file_list()

    client.request_file(files[0])
    client.get_file_list()

    client.delete_file(files[0])
    client.request_file(files[0])


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(e)
    except:
        print("Unknown error")
