Solution for tile++
1. host or referer header will leave a freed chunk
2. base64 decoded string will overflow the next chunk (the last remainder chunk ) size if the length of encode string is not divisible by 4
3. after being released, you have a big, overlap and freed chunk
4. the big chunk will be filled by post data, and the stdout ptr will be written
5. you can fake a file struct and fake a vtable to control the pc

