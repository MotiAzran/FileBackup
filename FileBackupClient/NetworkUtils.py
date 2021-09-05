def validate_ip(ip):
    """
    Check IP address is in the correct format
    :param ip: IP address to check
    :return: True if ip is valid, else False
    """
    for x in ip.split('.'):
        if not x.isdigit() or not 0 <= int(x) <= 255:
            return False

    return True


def validate_port(port):
    """
    Check port is valid
    :param port: port to check
    :return: True if port is valid, else False
    """
    MAX_PORT = 0xffff

    if not port.isdigit() or not 0 <= int(port) <= MAX_PORT:
        return False

    return True
