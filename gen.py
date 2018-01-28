from random import randint

# for test in xrange(1, 11):
    # with open('tests/%d.in' % test, 'w') as f:
        # for i in xrange(10000):
            # f.write('%d\n' % randint(0, 65536))

for test in xrange(11, 13):
    with open('tests/%d.in' % test, 'w') as f:
        for i in xrange(10):
            for j in xrange(0, 1000):
                f.write('%d\n' % randint(i * 3000, (i + 1) * 3000))
