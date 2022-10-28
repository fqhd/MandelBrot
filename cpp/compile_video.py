import os
import moviepy.video.io.ImageSequenceClip

image_folder='frames'
fps=30

image_files = [os.path.join(image_folder,img)
               for img in os.listdir(image_folder)
               if img.endswith(".jpg")]

def sortItems(items):
	return int(items[7:][:-4])

image_files.sort(key=sortItems)

clip = moviepy.video.io.ImageSequenceClip.ImageSequenceClip(image_files, fps=fps)
clip.write_videofile('my_video.mp4')