#!/bin/python

from datetime import date
import itertools
import os
import sys

class Embed:
	def __init__(self, file_path):
		basename = os.path.splitext(os.path.basename(file_path))[0]
		self.path = file_path
		self.basename = basename.replace('-', '_')
		self.signature = "void Get%s(unsigned int* size, const char** data)" % self.basename
		self.symbol = "s%sData" % self.basename

	def load_data(self):
		try:
			f = open(self.path, "rb")
			data = f.read()
			f.close()
			return data
		except Exception as e:
			print(e)
			sys.exit()

# itertools.batched requires python 3.12
def batched(iterable, size):
	N = len(iterable)
	for idx in range(0, N, size):
	    yield iterable[idx : min(idx + size, N)]

def gen_cpp(header, embeds):
	src = str(header)
	src += "#pragma warning(push)\n"
	src += "#pragma warning(disable : 4838)\n"
	src += "namespace LD {\n"
	src += "namespace Embed {\n"

	for embed in embeds:
		data = embed.load_data()
		src += "static const char %s[] = {\n" % (embed.symbol)
		
		for batch in batched(data, 128):
			for byte in batch:
				src += "0x%02x," % (byte)
			src += "\n"

		src += "};\n"

	for embed in embeds:
		src += embed.signature + "\n"
		src += "{\n"
		src += "    *size = sizeof(%s);\n" % (embed.symbol)
		src += "    *data = %s;\n" % (embed.symbol)
		src += "}\n"

	src += "} // namespace Embed\n"
	src += "} // namespace LD\n"
	src += "#pragma warning(pop)\n"

	return src


def main():
	if len(sys.argv) < 3:
		print("usage: %s [output file stem] [input file paths]", sys.argv[0])
		return
	
	output_name = sys.argv[1]
	input_files = sys.argv[2:]
	embeds = [Embed(input_file) for input_file in input_files]

	header  = "// generated by Embed.py, do not modify by hand\n"
	header += "// generated on: " + str(date.today()) + "\n"

	for embed in embeds:
		header += "// providing definition for LD::Embed::Get" + embed.basename + "\n"

	cpp = gen_cpp(header, embeds)

	try:
		f = open(output_name + ".cpp", "w")
		f.write(cpp)
		f.close()
		print("Embed.py: generated " % output_name)
	except Exception as e:
		print(e)
		sys.exit()

if __name__ == '__main__':
	main()