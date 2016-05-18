function SaveVid(frames, filename, fps)

writerObj = VideoWriter(filename);
writerObj.FrameRate = fps;
open(writerObj);
for k = 1:length(frames) 
   writeVideo(writerObj,frames(k));
end

close(writerObj);