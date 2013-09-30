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
    for y in xrange(img.size[1]):
      img.putpixel((x, y), 1)
    img.save('Images/test/vertical/line' + str(x) + '.bmp')


create_white_and_black()
create_half_and_half()
create_diagonals()
create_vertical_line_sequence()