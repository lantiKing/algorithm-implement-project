$debug = false

#interpolation type
NEAREST_NEIGHBOR = 1
BILINEAR = 2

#=begin
m = [ 8, 6, 13, 9, 
			 1,13,1, 15,
			 5, 4, 7,7,
			 5,10,3, 7]
#=end

=begin
m = [64, 2, 3,61,60, 6, 7,57,
			 9,55,54,12,13,51,50,18,
			17,47,46,20,21,43,42,24,
			40,26,27,37,36,30,31,33,
			32,34,35,29,28,38,39,25,
			41,23,22,44,45,19,18,48,
			49,15,14,52,53,11,10,56,
			 8,58,59, 5, 4,62,63, 1]
=end

def zero_interleave(img)
	img_width=Math.sqrt(img.length)
	m2_height=Math.sqrt(img.length)*2
	m2_width=Math.sqrt(img.length)*2
	m2 = Array.new(m2_height*m2_width)
	for i in 0..m2_height-1
		for j in 0..m2_width-1
			if(i%2==0 && j%2==0) then
				m2[i*m2_width+j] = img[(i/2)*img_width+j/2]
			else
				m2[i*m2_width+j] = 0
			end
		end
	end
	return m2
end

def interpolation(img,type)
	w = 1
	#filter_width = (2w+1)x(2w+1)行列
	filter_width = 2*w+1

	p = [1.0,1.0,0.0,
			 1.0,1.0,0.0,
			 0.0,0.0,0.0]

	q = [1,2,1,
			 2,4,2,
			 1,2,1]
	for i in 0..q.length-1
		q[i] = 0.25*q[i]
	end

	if(type==1) then
		filter = p
	elsif(type==2) then
		filter = q
	end

	img_width = Math.sqrt(img.length)
	img_height = Math.sqrt(img.length)

	new_img = Array.new(img_width,img_height)
	for y in 0..img_height-1
		for x in 0..img_width-1
			new=0
			for f_y in -w..w
				for f_x in -w..w
if($debug) then
					printf("img(%d,%d)[%f]  ",x+f_x,y+f_y,img[(y+f_y)*img_width+(x+f_x)])
					printf("filter(%d,%d)[%f] ",f_x+w,f_y+w,filter[(f_y+w)*p_width+(f_x+w)])
end
					if((y+f_y)>=0 && (x+f_x)>=0 && (y+f_y)*img_width+(x+f_x) < img.length)
						new = new + img[(y+f_y)*img_width+(x+f_x)]*filter[(f_y+w)*filter_width+(f_x+w)]
if($debug) then
						printf("--ok : %d",img[(y+f_y)*img_width+(x+f_x)]*filter[(f_y+w)*filter_width+(f_x+w)])
end
					end
if($debug) then	
					puts ''
end
				end
			end
			new_img[y*img_width+x]=new
if($debug) then
			printf("(%d,%d)=%f\n\n",x,y,new)
end
		end
	end
	return new_img
end

def print_img(img)
	img_width = Math.sqrt(img.length)
	img_height = Math.sqrt(img.length)

	printf("size:%dx%d\n",img_height,img_width)

	for i in 0..img_height-1
		for j in 0..img_width-1
			if(img[i*img_width+j]<10)then
				printf(" %2.2f,",img[i*img_width+j])
			else
				printf("%2.2f,",img[i*img_width+j])
			end
		end
		puts ''
	end
	puts ''
end

puts '-- src image --'
print_img(m)

puts '-- zero interleave image --'
m2 = zero_interleave(m)
print_img(m2)

puts '-- Nearest Neighbor interpolation image --'
m2p = interpolation(m2,NEAREST_NEIGHBOR)
print_img(m2p)

puts '-- Bilinear interpolation image --'
m2q = interpolation(m2,BILINEAR)
print_img(m2q)

