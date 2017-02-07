import csv
with open('memory-format.csv', 'rt') as csvfile:
  result = ''
  reader = csv.reader(csvfile, delimiter=',', quotechar='"')
  i = 0
  for row in reader:
    result += '<tr>'
    if(i == 0):
      for val in row:
        result += '<th>' + str(val) + '</th>'
    else:
      result += '<th>' + str(row[0]) + '</th>'
      for val in row[1:]:
        result += '<td>' + str(val) + '</td>'
    result += '</tr>\n'
    i += 1

print(result)

