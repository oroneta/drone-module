import os, time, datetime, re

'''
    Function to rotate log files

    :param str log_file: Path to log file
    :param int max_size: Max size of log file in bytes
    :param int max_days: Max number of log files
    :return: None
    :rtype: None

    :Example:
'''
def logRotate(path: str, filename: str, max_size: int, max_days: int) -> None:
    log_file = os.path.join(path, filename)

    # if file not exist
    if not os.path.isfile(log_file):
        open(log_file, 'a').close()
        return None

    # Get log file size
    log_size = os.path.getsize(log_file)

    # Remove old
    today = datetime.datetime.now()
    for f_n in os.listdir(path):
        match = re.match(r'(\d{8})', f_n.split('.')[-1])
        if match:
            date_str = match.group(1)
            file_date = datetime.datetime.strptime(date_str, '%Y%m%d')

            if (today - file_date).days > max_days:
                os.remove(os.path.join(path, f_n))

    if log_size > max_size:
        # If log file size is greater than max_size, rotate log files with date
        os.rename(log_file, f'{log_file}.{time.strftime("%Y%m%d")}')
        # And create a new log file
        open(log_file, 'a').close()
    return None