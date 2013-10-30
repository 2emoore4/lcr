import Image
import math
import os
import time

from optparse import OptionParser

b = 72 # baseline distance
f = 22 # focal length

left_theta = 1.5708
right_theta = 1.01129
left_alpha = 0.23022
right_alpha = 0.33389
frame_count = 96

image_size_x = 640
image_size_y = 480

sub_sampling_rate = 4
expected_x_deviation = 100

def rename_files(dir_name):
	if not os.listdir(dir_name):
		print "directory is empty, dummy"
	else:
		i = 1
		for filename in os.listdir(dir_name):
			frame_number = str(i).zfill(3)
			os.rename(dir_name + filename, dir_name + "scan" + frame_number + ".png")
			i += 1

def scan_dir(dir_name):
	print "Processing scans in directory: " + dir_name
	print_divider()
	if not os.listdir(dir_name):
		print "Directory is empty, make sure there are scans here."
	else:
		first_scan = Image.open(dir_name + os.listdir(dir_name)[1])
		z_array = [[-99999 for x in xrange(first_scan.size[1])] for x in xrange(first_scan.size[0])]

		print "Scanning images and creating z array..."
		print_divider()

		start_time = time.clock()

		scan_number = 0
		for filename in sorted(os.listdir(dir_name)):
			if ".png" in filename:
				scan_image_by_dev(dir_name + filename, scan_number, z_array)
				scan_number += 1

		output_pcd(z_array)

		end_time = time.clock()
		total_time = end_time - start_time
		print "scan finished in " + str(total_time) + " seconds."
		print str(total_time / len(os.listdir(dir_name))) + " seconds per frame."

def scan_video(file_name):
	print "going to scan video file: " + file_name

def scan_image(filename, scan_number, z_array):
	print "opening file " + filename
	scan = Image.open(filename)

	start_time = time.clock()

	previous_line_location = -1
	for y in xrange(0, scan.size[1], sub_sampling_rate):
		enter_white, exit_white = 0, 0

		if previous_line_location == -1 or previous_line_location < expected_x_deviation or previous_line_location > (image_size_x - expected_x_deviation):
			range = xrange(scan.size[0])
		else:
			range = xrange(previous_line_location - expected_x_deviation, previous_line_location + expected_x_deviation)

		for x in range:
			r, g, b = scan.getpixel((x, y))
			if is_white(r, g, b):
				enter_white = x
				break
		for x in xrange(enter_white, scan.size[0]):
			r, g, b = scan.getpixel((x, y))
			if not is_white(r, g, b):
				exit_white = x - 1
				break

		if enter_white != 0 and exit_white != 0:
			mid_white = int((enter_white + exit_white) / 2)
			previous_line_location = mid_white
			z_array[mid_white][y] = z_triangulation(mid_white, y, scan_number)

	end_time = time.clock()
	total_time = end_time - start_time
	print "scanned image in " + str(total_time) + " seconds"

def scan_image_by_dev(filename, scan_number, z_array):
	print "opening file " + filename
	scan = Image.open(filename)

	start_time = time.clock()

	first_line_location = -1
	for y in xrange(0, image_size_y, sub_sampling_rate):
		enter_white, exit_white = 0, 0

		for x in xrange(image_size_x):
			r, g, b = scan.getpixel((x, y))
			if is_white(r, g, b):
				enter_white = x
				break

		for x in xrange(enter_white, image_size_x):
			r, g, b = scan.getpixel((x, y))
			if not is_white(r, g, b):
				exit_white = x - 1
				break

		if enter_white != 0 and exit_white != 0:
			mid_white = int((enter_white + exit_white) / 2)

			if first_line_location == -1:
				z_array[mid_white][y] = float(0)
				first_line_location = mid_white
			else:
				xdiff = mid_white - first_line_location
				z_array[mid_white][y] = float(xdiff)

	end_time = time.clock()
	total_time = end_time - start_time
	print "scanned image in " + str(total_time) + " seconds"

def z_triangulation(x, y, scan_number):
	y = image_size_y - y
	theta = translate(scan_number, 0, frame_count - 1, left_theta, right_theta)
	alpha = translate(scan_number, 0, frame_count - 1, left_alpha, right_alpha)
	z = b * (math.sin(theta) / math.sin(alpha + theta))
	return z * (-f)

def translate(value, leftMin, leftMax, rightMin, rightMax):
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    valueScaled = float(value - leftMin) / float(leftSpan)

    return rightMin + (valueScaled * rightSpan)

def output_pcd(z_array):
	z_list = []
	for u in xrange(len(z_array)):
		for v in xrange(len(z_array[0])):
			point = z_array[u][v]
			if (point != -99999):
				z_list.append(point)

	pcd_file = open("output.pcd", "w")
	pcd_file.write("# .PCD v.7 -- file generated by structured_light_scan.py\n")
	pcd_file.write("VERSION .7\n")
	pcd_file.write("FIELDS x y z\n")
	pcd_file.write("SIZE 8 8 8\n")
	pcd_file.write("TYPE F F F\n")
	pcd_file.write("COUNT 1 1 1\n")
	pcd_file.write("WIDTH " + str(len(z_list)) + "\n")
	pcd_file.write("HEIGHT 1\n")
	pcd_file.write("VIEWPOINT 0 0 0 1 0 0 0\n")
	pcd_file.write("POINTS " + str(len(z_list)) + "\n")
	pcd_file.write("DATA ascii\n")

	for x in xrange(len(z_array)):
		for y in xrange(len(z_array[0])):
			if z_array[x][y] != -99999:
				pcd_file.write(str(x) + " " + str(image_size_y - y) + " " + str(z_array[x][y]) + "\n")

def is_white(r, g, b):
	if r == 255 and g == 255 and b == 255:
		return True
	else:
		return False

def print_divider():
	print "-----------------------------------------------------"

def fix_dir_name(dir_name):
	if dir_name.endswith("/"):
		return dir_name
	else:
		return dir_name + "/"

def main():
	parser = OptionParser()
	parser.add_option("-r", action="store_true", dest="rename")
	parser.add_option("-s", action="store_true", dest="scan")
	parser.add_option("-d", "--dir", dest="dir_name")
	parser.add_option("-f", "--file", dest="file_name")

	(options, args) = parser.parse_args()

	if not options.dir_name or options.file_name:
		print "Need to specify directory with -d option"
	elif options.dir_name:
		fixed_dir = fix_dir_name(options.dir_name)

		if options.rename:
			rename_files(fixed_dir)

		if options.scan:
			scan_dir(fixed_dir)
	elif options.file_name:
		if options.scan:
			scan_video(options.file_name)

if __name__ == "__main__":
	main()
