#!/usr/bin/env python3
import os
import sys
import http.cookies
import uuid
import time

SESSION_DIR = "/tmp/pycgi_sessions"
SESSION_LIFETIME = 60 * 30  # 30分

def ensure_session_dir():
    if not os.path.exists(SESSION_DIR):
        os.makedirs(SESSION_DIR, exist_ok=True)

def load_session(sid):
    path = os.path.join(SESSION_DIR, sid)
    if not os.path.isfile(path):
        return {}
    if time.time() - os.path.getmtime(path) > SESSION_LIFETIME:
        os.remove(path)
        return {}
    with open(path, "r") as f:
        data = {}
        for line in f:
            if '=' in line:
                k, v = line.strip().split('=', 1)
                data[k] = v
        return data

def save_session(sid, data):
    path = os.path.join(SESSION_DIR, sid)
    with open(path, "w") as f:
        for k, v in data.items():
            f.write(f"{k}={v}\n")

def get_cookie_sid():
    if 'HTTP_COOKIE' in os.environ:
        cookie = http.cookies.SimpleCookie(os.environ['HTTP_COOKIE'])
        if 'SID' in cookie:
            return cookie['SID'].value
    return None

def generate_sid():
    return uuid.uuid4().hex

def print_headers(sid):
    cookie = http.cookies.SimpleCookie()
    cookie['SID'] = sid
    cookie['SID']['path'] = '/'
    cookie['SID']['max-age'] = str(SESSION_LIFETIME)

    sys.stdout.write("Content-Type: text/html\r\n")
    for morsel in cookie.values():
        sys.stdout.write(f"Set-Cookie: {morsel.OutputString()}\r\n")
    sys.stdout.write("\r\n")  # ヘッダとボディの区切り

def main():
    ensure_session_dir()
    sid = get_cookie_sid()
    if not sid:
        sid = generate_sid()
        session = {}
    else:
        session = load_session(sid)

    count = int(session.get("count", "0")) + 1
    session["count"] = str(count)
    save_session(sid, session)

    print_headers(sid)

    sys.stdout.write(f"""<html>
<head><meta charset="utf-8"><title>Session Test</title></head>
<body>
    <h1>セッションカウント: {count}</h1>
    <form method="post">
        <input type="submit" value="カウントアップ">
    </form>
</body>
</html>
""")

if __name__ == "__main__":
    main()
