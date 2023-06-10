import sys
import os

print("Content-Type: text/html; charset=utf-8\n\n")

print("<html lang=\"ja\"><body>")

print("<h1>Python Version</h1>")
print("<p>{}</p>".format(sys.version))

print("<h1>System info</h1>")
print("<p>Pratform: {}</p>".format(sys.platform))
print("<p>PWD: {}</p>".format(os.getcwd()))
print("<p>ENV: {}</p>".format(os.environ))

print("</body></html>")
