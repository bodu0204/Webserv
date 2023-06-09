#!/usr/bin/env python
import sys
import os

# ヘッダーの出力
print("Content-Type: text/html; charset=utf-8\n\n")

# HTMLの出力
print("<html lang=\"ja\"><body>")

# Pythonのバージョン情報の出力
print("<h1>Pythonのバージョン情報</h1>")
print("<p>{}</p>".format(sys.version))

# システムの情報の出力
print("<h1>システムの情報</h1>")
print("<p>プラットフォーム: {}</p>".format(sys.platform))
print("<p>カレントディレクトリ: {}</p>".format(os.getcwd()))
print("<p>環境変数: {}</p>".format(os.environ))

print("</body></html>")
