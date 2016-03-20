# pi-funcs.py
# File containing useful functions

# function to remap one range to another
def remap(value, fromLow, fromHigh, toLow, toHigh):
        # get how wide each range is
        fromRange = fromHigh - fromLow
        toRange = toHigh - toLow

        # convert low range into a 0-1 range
        valueNew = float(value - fromLow) / float(fromRange)

        # convert 0-1 range to high range
        return toLow + (valueNew * toRange)
# end remap()

