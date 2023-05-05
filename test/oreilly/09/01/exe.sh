#!/bin/bash
echo 'go run main.go;';
go run main.go;

echo 'GODEBUG=http2client=1 go run main.go;';
GODEBUG=http2client=1 go run main.go;

echo 'GODEBUG=http2debug=1 go run main.go ;';
GODEBUG=http2debug=1 go run main.go ;

