fn test use a, b:
  if a = b do
    display "a equals b"
  else do
    display "a does not equal b"
  end if
end fn

for each a in [1, 2, 3, 4, 5]
  for each b in [1, 2, 3, 4, 5]
    test use a, b
  end for
end for

launch test using 1, 2

display "done"
