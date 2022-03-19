# @see https://www.g200kg.com/jp/docs/tech/notefreq.html

def freq key
  @frequencies[@notes.index(key)]
end

def note key
  @notes.index(key)
end

def define_frequenceies
  tuning = 440.0
  keys = %w(C C# D D# E F F# G G# A A# B)
  @frequencies = []
  @notes = []
  128.times do |i|
    oct = i / 12 - 1
    key = i % 12
    freq = (tuning * (2.0 ** ((i - 69.0) / 12.0)) + 0.5).to_i
    @frequencies << freq
    @notes << "#{keys[key]}#{oct}"
  end
  @notes << 'R'
  a = <<EOS
#ifndef _FREQ_H
#define _FREQ_H
EOS
  a = [a]
  a <<"uint16_t frequencies[] = {"
  @frequencies.each_slice(12) do |fs|
    a << "\t#{fs.map(&:to_s).join(", ")},"
  end
  a << "};"
  a << ""
  a << "const char *note_names[] = {"
  @notes.each_slice(12) do |nn|
    a << "\t#{nn.map{|n| "\"#{n}\""}.join(", ")},"
  end
  a << "};"
  a << ""
  a << "#endif // _FREQ_H"
  File.write("src/freq.h", a.join("\n") + "\n")
end

def define_melody melody
  oct = "4"
  keys = melody.scan(/[ABCDEFG]#?\-?\d?|R/).map{|k| m = /(\-?\d)/ =~ k; oct = $1 if $1; m ? k : k == 'R' ? k : "#{k}#{oct}"}
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
  a << ""
  a << "#endif // _MELODY_H"
  File.write("src/melody.h", a.join("\n") + "\n")
end

define_frequenceies

zousan = "F4DCR FDCR FGAC5A4AGFG C5CA4 D5CA4F GADCFR"
edelweiss = <<EOS
E4G D5 CG4 FR EE
EFG A GR EG D5 C
G4 FR EG GAB C5 CR
DRG4G BAG EG C5R A4C5
DC B4 GR EG D5 CG4
FR EG GAB C5 CR
EOS

#define_melody zousan
define_melody edelweiss