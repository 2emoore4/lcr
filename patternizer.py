import Image

def create_white_and_black():
  img = Image.new('1', (608, 684), 'white')
  img.save('Images/test/white.bmp', 'BMP')

  img = Image.new('1', (608, 684), 'black')
  img.save('Images/test/black.bmp', 'BMP')

def create_half_and_half():
  img = Image.new('1', (608, 684), 'white')
  for y in xrange(img.size[1] / 2):
    for x in xrange(img.size[0]):
      img.putpixel((x, y), 0)
  img.save('Images/test/half1.bmp')

  img = Image.new('1', (608, 684), 'black')
  for y in xrange(img.size[1] / 2):
    for x in xrange(img.size[0]):
      img.putpixel((x, y), 1)
  img.save('Images/test/half2.bmp')

def create_diagonals():
  img = Image.new('1', (608, 684), 'white')
  for y in xrange(img.size[1] / 2):
    for x in xrange(img.size[0] / 2, img.size[0]):
      img.putpixel((x, y), 0)
  for y in xrange(img.size[1] / 2, img.size[1]):
    for x in xrange(img.size[0] / 2):
      img.putpixel((x, y), 0)
  img.save('Images/test/diag1.bmp')

  img = Image.new('1', (608, 684), 'black')
  for y in xrange(img.size[1] / 2):
    for x in xrange(img.size[0] / 2, img.size[0]):
      img.putpixel((x, y), 1)
  for y in xrange(img.size[1] / 2, img.size[1]):
    for x in xrange(img.size[0] / 2):
      img.putpixel((x, y), 1)
  img.save('Images/test/diag2.bmp')

def create_vertical_line_sequence():
  for x in xrange(608):
    img = Image.new('1', (608, 684), 'black')
    draw_white_line_at_x(img, [x])
    img.save('Images/test/vertical/line' + str(x) + '.bmp')

def create_vert_seven():
  for frame in xrange(86): # for each of 86 frames
    img = Image.new('1', (608, 684), 'black')
    for line in xrange(7): # for each of 7 lines
      # draw vertical stripe at ((line * 86) + frame)
      x = (line * 86) + frame
      draw_white_line_at_x(img, [x])
    img.save('Images/test/vert_seven/line' + str(frame) + '.bmp')

def create_weird_code():
  img = Image.new('1', (608, 684), 'black')
  draw_white_line_at_x(img,
[1,2,4,7,9,10,13,14,16,19,20,23,25,26,28,31,33,34,36,39,40,42,45,46,48,51,52,54,57,59,60,62,65,67,69,70,72,75,77,78,81,83,84,86,89,90,93,95,96,98,101,102,104,107,109],
5)
  img.save('Images/test/code/frame00.bmp')

  img = Image.new('1', (608, 684), 'black')
  draw_white_line_at_x(img,
[2,4,5,7,10,12,13,15,18,20,21,23,26,28,29,32,34,35,37,40,41,43,46,47,49,52,53,55,58,59,61,64,66,67,69,72,74,75,77,80,82,83,86,87,89,92,93,95,98,100,101,103,106,108,109], 5)
  img.save('Images/test/code/frame01.bmp')

  img = Image.new('1', (608, 684), 'black')
  draw_white_line_at_x(img,
[1,3,4,7,9,10,13,15,16,18,21,23,24,26,29,30,32,35,37,38,40,43,45,46,48,51,53,54,56,59,60,62,65,66,68,71,73,74,77,78,80,83,85,86,88,91,93,94,96,99,100,102,105,107,108], 5)
  img.save('Images/test/code/frame02.bmp')

  img = Image.new('1', (608, 684), 'black')
  draw_white_line_at_x(img,
[2,3,5,8,10,11,13,16,18,19,22,23,25,28,30,31,34,35,37,40,42,43,45,48,50,51,53,56,58,59,61,64,65,67,70,71,73,76,78,79,81,84,86,87,89,92,94,95,97,100,102,103,105,108,109], 5)
  img.save('Images/test/code/frame03.bmp')

def draw_white_line_at_x(image, x_locations, size = 1):
  for loc in x_locations:
    for x in xrange(loc * size, (loc * size) + size):
      for y in xrange(image.size[1]):
        image.putpixel((x, y), 1)

def location_of_stripe(stripe_num):
  return stripe * 5

create_white_and_black()
create_half_and_half()
create_diagonals()
create_vertical_line_sequence()
create_vert_seven()
create_weird_code()
