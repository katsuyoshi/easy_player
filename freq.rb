# @see https://msr-r.net/m5stack-sound/
# A0 - C8
@frequencies = %w(
  27.5
  29.135
  30.868
  32.703
  34.648
  36.708
  38.891
  41.203
  43.654
  46.249
  48.999
  51.913
  55
  58.27
  61.735
  65.406
  69.296
  73.416
  77.782
  82.407
  87.307
  92.499
  97.999
  103.826
  110
  116.541
  123.471
  130.813
  138.591
  146.832
  155.563
  164.814
  174.614
  184.997
  195.998
  207.652
  220
  233.082
  246.942
  261.626
  277.183
  293.665
  311.127
  329.628
  349.228
  369.994
  391.995
  415.305
  440
  466.164
  493.883
  523.251
  554.365
  587.33
  622.254
  659.255
  698.456
  739.989
  783.991
  830.609
  880
  932.328
  987.767
  1046.502
  1108.731
  1174.659
  1244.508
  1318.51
  1396.913
  1479.978
  1567.982
  1661.219
  1760
  1864.655
  1975.533
  2093.005
  2217.461
  2349.318
  2489.016
  2637.02
  2793.826
  2959.955
  3135.963
  3322.438
  3520
  3729.31
  3951.066
  4186.009
).map{|f| (f.to_i + 0.5).to_i }

scale = %w(A A# B C C# D D# E F F# G G#)
@notes = 9.times.map do |o|
  scale.map{|s| "#{s}#{o}"}
end.flatten

def freq key
  @frequencies[@notes.index(key)]
end

def note key
  @notes.index(key)
end


def define_frequenceies
  a = <<EOS
#ifndef _FREQ_H
#define _FREQ_H
EOS
  a = [a]
  a <<"uint16_t frequencies[] = {"
  @frequencies.each_slice(12).each do |fs|
    a << "\t#{fs.map(&:to_s).join(", ")},"
  end
  a << "};"
  a << "#endif"
  File.write("src/freq.h", a.join("\n") + "\n")
end

def define_melody melody
  oct = "4"
  keys = melody.scan(/[ABCDEFG]#?\d?/).map{|k| m = /(\d)/ =~ k; oct = $1 if $1; m ? k : "#{k}#{oct}"}
  a = <<EOS
#ifndef _MELODY_H
#define _MELODY_H
EOS
  a = [a]
  a << "int16_t notes[] = {"
  keys.map{|k| note k}.each_slice(8) do |ns|
    a << "\t#{ns.map(&:to_s).join(", ")},"
  end
  a << "};"
  a << "#endif"
  File.write("src/melody.h", a.join("\n") + "\n")
end

define_frequenceies

zousan = "F4DC FDC FGAC5A4AGFG C5CA4 D5CA4F GADCF"

define_melody zousan