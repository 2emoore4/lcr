import Image
import math
import os
import sys
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

sub_sampling_rate = 1
expected_x_deviation = 100
maximum_line_size = 15
gradient_threshold = 165
average_stripe_width = 6

def rename_files(dir_name):
    if not os.listdir(dir_name):
        print "directory is empty, dummy"
    else:
        i = 1
        for filename in os.listdir(dir_name):
            frame_number = str(i).zfill(3)
            os.rename(dir_name + filename, dir_name + "scan" + frame_number + ".png")
            i += 1

def scan_dir(dir_name, calibration_dir = None):
    print "Processing scans in directory: " + dir_name
    print_divider()
    if not os.listdir(dir_name):
        print "Directory is empty, make sure there are scans here."
    else:
        if calibration_dir:
            cal_array = calibrate_from_dir(calibration_dir)
        else:
            cal_array = None

        first_scan = Image.open(dir_name + os.listdir(dir_name)[1])
        z_array = [[-99999 for x in xrange(first_scan.size[1])] for x in xrange(first_scan.size[0])]

        print "Scanning images and creating z array..."
        print_divider()

        start_time = time.clock()

        scan_number = 0
        for filename in sorted(os.listdir(dir_name)):
            if ".png" in filename:
                scan_image_by_dev(dir_name + filename, scan_number, z_array, cal_array)
                scan_number += 1

        output_pcd(z_array)

        end_time = time.clock()
        total_time = end_time - start_time
        print "scan finished in " + str(total_time) + " seconds."
        print str(total_time / len(os.listdir(dir_name))) + " seconds per frame."

def scan_dir_with_code(dir_name, calibration_dir):
    print "Processing scans in directory: " + dir_name
    print_divider()
    if not os.listdir(dir_name) or not os.listdir(calibration_dir):
        print "Directory is empty, make sure there are scans here."
    else:
        start_time = time.clock()
        cal_array = calibrate_from_code(calibration_dir)
        end_time = time.clock()
        total_time = end_time - start_time
        print "calibration finished in " + str(total_time) + " seconds."

        first_scan = Image.open(dir_name + os.listdir(dir_name)[1])
        z_array = [[-99999 for x in xrange(first_scan.size[1])] for x in xrange(first_scan.size[0])]

        print "Scanning images and creating z array..."
        print_divider()

        start_time = time.clock()

        boundary_buffer, scan_buffer = [], []
        for filename in sorted(os.listdir(dir_name)):
            if ".png" in filename:
                scan_image_from_code(dir_name + filename, z_array, cal_array, boundary_buffer, scan_buffer)

        output_pcd(z_array)

        end_time = time.clock()
        total_time = end_time - start_time
        print "scan finished in " + str(total_time) + " seconds."
        print str(total_time / len(os.listdir(dir_name))) + " seconds per frame."

def calibrate_from_dir(dir_name):
    print "Calibrating scaner with frames in directory: " + dir_name
    print_divider()
    if not os.listdir(dir_name):
        print "Calibration directory is empty, make sure there are scans here."
    else:
        first_scan = Image.open(dir_name + os.listdir(dir_name)[1])
        cal_array = [[-99999 for x in xrange(first_scan.size[1])] for x in xrange(len(os.listdir(dir_name)))]

        print "Scanning images and creating calibration array..."
        print_divider()

        scan_number = 0
        for filename in sorted(os.listdir(dir_name)):
            if ".png" in filename:
                calibrate_with_image(dir_name + filename, scan_number, cal_array)
                scan_number += 1

        print "Finished calibration."
        print_divider()

        return cal_array

def calibrate_from_code(dir_name):
    print "Calibrating scanner with frames that contain gray code, located in directory: " + dir_name
    print_divider()
    if not os.listdir(dir_name):
        print "Calibration directory is empty, make sure there are scans here."
    else:
        print "Scanning images and creating calibration array..."
        print_divider()

        scan_names = []
        for filename in sorted(os.listdir(dir_name)):
            if ".png" in filename:
                scan_names.append(filename)

        if len(scan_names) != 4:
            print "there need to be 4 scans in this directory"
            sys.exit(0)

        calibration_boundaries, scans = [], []

        for filename in scan_names:
            scan = Image.open(dir_name + filename)
            calibration_boundaries.append(find_boundaries_in_image(scan))
            scans.append(scan)

        boundary_locations, boundary_to_location = [], []
        for y in xrange(image_size_y):
            bound_one = add_possible_ghosts(calibration_boundaries[0][y])
            bound_two = add_possible_ghosts(calibration_boundaries[1][y])
            matches = matching_boundaries(bound_two, bound_one)
            line_boundaries_to_location = []
            for pair in matches:
                pair_one, pair_two = pair
                location = bound_one[pair_two]
                line_boundaries_to_location.append((location, colors_opposite_boundary(bound_two[pair_one], y, scans)))
            boundary_to_location.append(line_boundaries_to_location)
        return boundary_to_location

def add_possible_ghosts(line_boundaries):
    current_length = len(line_boundaries)
    current_index = 1
    while current_index < current_length:
        if line_boundaries[current_index] - line_boundaries[current_index - 1] > average_stripe_width:
            ghost_location = int(round((line_boundaries[current_index] + line_boundaries[current_index - 1]) / 2))
            line_boundaries.insert(current_index, ghost_location)
            current_length += 1
            current_index += 1
        current_index += 1
    return line_boundaries

def colors_opposite_boundary(x, y, scans):
    left, right = [], []
    for scan in scans:
        l_r, l_g, l_b = scan.getpixel((x - 1, y))
        r_r, r_g, r_b = scan.getpixel((x + 1, y))
        left.append(is_white(l_r, l_g, l_b))
        right.append(is_white(r_r, r_g, r_b))
    return (left, right)

def matching_boundaries(first_list, second_list):
    boundaries = []
    for i in xrange(len(first_list)):
        boundary_in_first = first_list[i]
        match_in_second = boundary_in_list(boundary_in_first, second_list)
        if match_in_second != -1:
            pair = (i, match_in_second)
            boundaries.append(pair)
    return boundaries

def boundary_in_list(query, target_list, fuzziness = 1):
    index_in_target_list = find_closest_in_list(query, target_list)

    if index_in_target_list == -1:
        return -1

    difference = query - target_list[index_in_target_list]
    if math.fabs(difference) <= fuzziness:
        return index_in_target_list
    else:
        return -1

def find_closest_in_list(query, target_list):
    first_larger_value = -1
    for i in xrange(len(target_list)):
        if target_list[i] >= query:
            first_larger_value = i
            break

    if first_larger_value == -1:
        return len(target_list) - 1
    elif target_list[first_larger_value] == query or first_larger_value == 0:
        return first_larger_value

    # found index of a larger value. now query is in between f_l_v and f_l_v - 1
    low_value = target_list[first_larger_value - 1]
    high_value = target_list[first_larger_value]

    if query - low_value < high_value - query:
        return first_larger_value - 1
    else:
        return first_larger_value

def find_projection_in_line(scan, y_pix, est_location = -1):
    enter_white, exit_white = image_size_x, 0

    if est_location == -1 or est_location < expected_x_deviation or est_location > (image_size_x - expected_x_deviation):
        range = xrange(image_size_x)
    else:
        range = xrange(est_location - expected_x_deviation, est_location + expected_x_deviation)

    for x in range:
        r, g, b = scan.getpixel((x, y_pix))
        if is_white(r, g, b):
            enter_white = x
            break

    for x in xrange(enter_white, image_size_x):
        r, g, b = scan.getpixel((x, y_pix))
        if not is_white(r, g, b):
            exit_white = x - 1
            break

    if enter_white != image_size_x and exit_white != 0 and (exit_white - enter_white) < maximum_line_size:
        return int((enter_white + exit_white) / 2)
    else:
        return -99999


def calibrate_with_image(filename, scan_number, cal_array):
    print "Opening file " + filename
    scan = Image.open(filename)

    previous_line_location = -1
    for y in xrange(scan.size[1]):
        line_location = find_projection_in_line(scan, y, previous_line_location)
        if line_location != -99999:
            previous_line_location = line_location
            cal_array[scan_number][y] = line_location


def scan_video(file_name):
    print "going to scan video file: " + file_name

def scan_image_with_parallax(filename, scan_number, z_array):
    print "opening file " + filename
    scan = Image.open(filename)

    start_time = time.clock()

    for y in xrange(0, scan.size[1], sub_sampling_rate):
        line_location = find_projection_in_line(scan, y)
        if line_location != -99999:
            z_array[line_location][y] = z_triangulation(line_location, y, scan_number)

    end_time = time.clock()
    total_time = end_time - start_time
    print "scanned image in " + str(total_time) + " seconds"

def scan_image_by_dev(filename, scan_number, z_array, calibration_array = None):
    print "opening file " + filename
    scan = Image.open(filename)

    start_time = time.clock()

    first_line_location = -1
    previous_line_location = -1
    for y in xrange(0, image_size_y, sub_sampling_rate):
        line_location = find_projection_in_line(scan, y, previous_line_location)
        if line_location != -99999:
            previous_line_location = line_location
            if calibration_array:
                expected_location = calibration_array[scan_number][y]
                if expected_location != -99999:
                    xdiff = line_location - expected_location
                    z_array[line_location][y] = float(xdiff)
            else:
                if first_line_location == -1:
                    z_array[line_location][y] = float(0)
                    first_line_location = line_location
                else:
                    xdiff = line_location - first_line_location
                    z_array[line_location][y] = float(xdiff)

    end_time = time.clock()
    total_time = end_time - start_time
    print "scanned image in " + str(total_time) + " seconds"

def scan_image_from_code(filename, z_array, calibration_array, boundary_buffer, scan_buffer):
    print "opening file " + filename
    scan = Image.open(filename)

    start_time = time.clock()

    all_boundaries = find_boundaries_in_image(scan)
    boundaries_with_ghosts = []
    for y in xrange(image_size_y):
        line_bounds = add_possible_ghosts(all_boundaries[y])
        boundaries_with_ghosts.append(line_bounds)

    # add or add+remove to buffer
    buffer_size = len(boundary_buffer)
    boundary_buffer.append(boundaries_with_ghosts)
    scan_buffer.append(scan)
    if buffer_size == 5:
        boundary_buffer.pop(0)
        scan_buffer.pop(0)
    elif buffer_size > 5:
        print "ERROR: scan buffer overfloweth for some reason"
        sys.exit(0)

    if len(boundary_buffer) == 4:
        for y in xrange(0, image_size_y, sub_sampling_rate):
            matches = matching_boundaries(boundary_buffer[3][y], boundary_buffer[2][y])
            line_boundaries_to_location = []
            for pair in matches:
                pair_one, pair_two = pair
                location = boundary_buffer[3][y][pair_one]
                line_boundaries_to_location.append((location, colors_opposite_boundary(boundary_buffer[3][y][pair_one], y, scan_buffer)))
            for boundary in line_boundaries_to_location:
                # match to boundary in calibration_array
                for i in xrange(len(calibration_array[y])):
                    cal_boundary = calibration_array[y][i]
                    cal_location, cal_history = cal_boundary
                    cur_location, cur_history = boundary
                    if cur_history == cal_history:
                        xdiff = cur_location - cal_location
                        z_array[cur_location][y] = float(xdiff)

    end_time = time.clock()
    total_time = end_time - start_time
    print "scanned image in " + str(total_time) + " seconds"

def print_boundaries(filename):
    print "showing boundaries for file " + filename
    scan = Image.open(filename)
    for y in xrange(image_size_y):
        print len(find_boundaries_in_line(scan, y))

def find_boundaries_in_image(scan):
    start_time = time.clock()
    boundary_lines = []
    for y in xrange(image_size_y):
        boundary_lines.append(find_boundaries_in_line(scan, y))
    end_time = time.clock()
    total_time = end_time - start_time
    print "boundaries found in " + str(total_time) + " seconds."
    return boundary_lines

def find_boundaries_in_line(scan, y_pix):
    gradient_list = []
    current_x = 0

    # this is similar to the find_projection_in_line method except it uses
    # this while loop instead of a for loop, in case there are more than
    # two boundaries in this line.
    while current_x < image_size_x:
        # these are for marking boundaries, if they are found.
        # enter_white is set to image_size_x so that, if the first for
        # loop reaches the end of the line without finding white, the
        # second for loop will not execute.
        enter_white, exit_white = image_size_x, 0

        for x in xrange(current_x, image_size_x):
            r, g, b = scan.getpixel((x, y_pix))
            if is_white(r, g, b):
                enter_white = x
                break

        for x in xrange(enter_white, image_size_x):
            r, g, b = scan.getpixel((x, y_pix))
            if not is_white(r, g, b):
                exit_white = x
                break

        if enter_white != image_size_x and exit_white != 0 and (enter_white - exit_white < maximum_line_size):
            # scan found a white line. append the beginning and end
            # of this line to the gradient list.
            gradient_list.append(enter_white)
            gradient_list.append(exit_white)
            current_x = exit_white + 1
        else:
            # scan reached the end of the line without finding
            # white. set current_x so that it doesn't satisfy the
            # while condition.
            current_x = image_size_x

    return gradient_list

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
    if r > 150 and g > 150 and b > 150:
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
    parser.add_option("-c", "--cal-dir", dest="cal_dir")
    parser.add_option("-b", action="store_true", dest="show_boundaries")

    (options, args) = parser.parse_args()

    if options.rename:
        if options.dir_name:
            fixed_dir = fix_dir_name(options.dir_name)
            rename_files(fixed_dir)
        else:
            print "need to specify a directory name with the rename option"
            sys.exit(0)
    elif options.scan:
        if options.dir_name:
            fixed_dir = fix_dir_name(options.dir_name)
            if options.cal_dir:
                scan_dir(fixed_dir, options.cal_dir)
            else:
                scan_dir(fixed_dir)
        else:
            print "need to specify a directory name with the scan option"
            sys.exit(0)
    elif options.show_boundaries:
        if options.dir_name and options.cal_dir:
            fixed_dir = fix_dir_name(options.dir_name)
            fixed_cal = fix_dir_name(options.cal_dir)
            scan_dir_with_code(fixed_dir, fixed_cal)
        elif options.file_name:
            print_boundaries(options.file_name)
        else:
            print "need to specify a directory or file name with the boundaries option"
            sys.exit(0)
    else:
        print "need to specify a scan option"
        sys.exit(0)

if __name__ == "__main__":
    main()
