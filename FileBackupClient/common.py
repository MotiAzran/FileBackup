MAX_BYTE = 0xff
MAX_WORD = 0xffff
MAX_DWORD = 0xffffffff


class CriticalException(Exception):
    def __init__(self, msg):
        super().__init__(msg)


class NonCriticalException(Exception):
    def __init__(self, msg):
        super().__init__(msg)


class FileNotFound(NonCriticalException):
    def __init__(self, filename):
        super().__init__("{} not found".format(filename))


class NoFiles(NonCriticalException):
    def __init__(self):
        super().__init__("Server has no files for user")


class GeneralServerError(CriticalException):
    def __init__(self):
        super().__init__("General server error")


class UnknownServerError(CriticalException):
    def __init__(self):
        super().__init__("Got unknown error from server")