#!/usr/bin/env python
import cgi, os
import cgitb;
cgitb.enable()
input_data = cgi.FieldStorage()
# Get filename here.
print (input_data)
print ("end")
fileitem = input_data["uploaded_file"]
# Test if the file was uploaded
if fileitem.filename:
    # fn = os.path.basename(fileitem.filename)
    fn = os.path.basename(fileitem.filename.replace("\\", "/" ))
    open('/tmp/' + fn, 'wb').write(fileitem.file.read())
    message = 'The file "' + fn + '" was uploaded successfully'
else:
    message = 'No file was uploaded'
print ("""Content-Type: text/html\n
            <html>
                <body>
                    <p>%s</p>
                </body>
            </html>
""" % (message,))