# Biquad filter class
class BiquadFilter:
    def __init__(self, b0, b1, b2, a1, a2):
        self.b0 = b0
        self.b1 = b1
        self.b2 = b2
        self.a1 = a1
        self.a2 = a2
        self.x1 = self.x2 = self.y1 = self.y2 = 0.0

    def process_sample(self, x0):
        y0 = self.b0*x0 + self.b1*self.x1 + self.b2*self.x2 - self.a1*self.y1 - self.a2*self.y2
        self.x2, self.x1 = self.x1, x0
        self.y2, self.y1 = self.y1, y0
        return y0
