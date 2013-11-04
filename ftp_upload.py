from ftplib import FTP
from getpass import getpass
from shutil import copy2

import os
import time

original_filename = "output.pcd"

def pcd_exists():
  return original_filename in os.listdir(".")

def rename_pcd():
  new_filename = time.strftime("%Y.%m.%d.%H.%M.%S") + ".pcd"
  os.rename(original_filename, new_filename)
  return new_filename

def copy_file(filename):
  copy2(filename, "/srv/pcd/" + filename)

def send_file(filename):
  ftppass = getpass()
  ftp = FTP("evanmoore.no-ip.org")
  ftp.sendcmd("USER root")
  ftp.sendcmd("PASS " + ftppass)
  ftp.cwd("/srv/pcd")
  ftp.storlines("STOR " + filename, os.fdopen(os.open(filename, os.O_RDONLY)))
  ftp.quit()
  print "sent file: " + filename
  os.remove(filename)

if pcd_exists():
  new_name = rename_pcd()
  copy_file(new_name)
  send_file(new_name)
else:
  print "no pcd file exists in this directory"

