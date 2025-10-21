#ifndef PX_UTIL_FILEIO_HPP
#define PX_UTIL_FILEIO_HPP

#include <filesystem>
#include <fstream>

#include "Exception.hpp"
#include "String.hpp"

namespace px
{
	namespace Util
	{
		class TextFile
		{
		public:
			const std::filesystem::path Filename() const { return filename_; }

			static void Create(const std::filesystem::path& filename, const std::vector<px::String>& lines)
			{
				if (std::filesystem::exists(filename))
				{
					if (std::filesystem::is_directory(filename))
						throw px::Exception(px::ErrorID::FileNotFound,
							{
								{ px::TagID::Filename, px::Str(filename.string()) },
								{ px::TagID::Callee, px::Str(__FUNCTION__) }
							});
				}

				std::wofstream file(filename.c_str());

				if (!file)
					throw px::Exception(px::ErrorID::FileRead,
						{
							{ px::TagID::Filename, px::Str(filename.string()) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});

				for (unsigned int ln = 0; ln < static_cast<unsigned int>(lines.size()); ++ln)
					file << lines[ln] << "\n";
			}

			static void Create(const std::filesystem::path& filename, const px::String& syntax)
			{
				if (std::filesystem::exists(filename))
				{
					if (std::filesystem::is_directory(filename))
						throw px::Exception(px::ErrorID::FileNotFound,
							{
								{ px::TagID::Filename, px::Str(filename.string()) },
								{ px::TagID::Callee, px::Str(__FUNCTION__) }
							});
				}

				std::wofstream file(filename.c_str());

				if (!file)
					throw px::Exception(px::ErrorID::FileRead,
						{
							{ px::TagID::Filename, px::Str(filename.string()) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});

				file << syntax;
			}

			void Write(const std::filesystem::path& filename, const std::vector<px::String>& lines)
			{
				TextFile file(filename);
				file.SetLines(lines);
				file.Write();
			}

			void Write(const std::filesystem::path& filename, const px::String& syntax)
			{
				TextFile file(filename);
				file.SetText(syntax);
				file.Write();
			}

			void Append(const std::filesystem::path& filename, const px::String& syntax)
			{
				if (std::filesystem::exists(filename))
				{
					if (std::filesystem::is_directory(filename))
						throw px::Exception(px::ErrorID::FileRead,
							{
								{ px::TagID::Filename, px::Str(filename.string()) },
								{ px::TagID::Callee, px::Str(__FUNCTION__) }
							});
				}
				else
					throw px::Exception(px::ErrorID::FileNotFound,
						{
							{ px::TagID::Filename, px::Str(filename.string()) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});

				std::wofstream file(filename.c_str(), std::ios::app);
				file << syntax;
			}

			void Append(const std::filesystem::path& filename, const std::vector<px::String>& lines)
			{
				if (std::filesystem::exists(filename))
				{
					if (std::filesystem::is_directory(filename))
						throw px::Exception(px::ErrorID::FileRead,
							{
								{ px::TagID::Filename, px::Str(filename.string()) },
								{ px::TagID::Callee, px::Str(__FUNCTION__) }
							});
				}
				else
					throw px::Exception(px::ErrorID::FileNotFound,
						{
							{ px::TagID::Filename, px::Str(filename.string()) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});

				std::wofstream file(filename.c_str(), std::ios::app);
				for (unsigned int ln = 0; ln < static_cast<unsigned int>(lines.size()); ++ln)
					file << lines[ln] << "\n";
			}

			TextFile(const std::filesystem::path& filename)
				: filename_(filename)
			{
				if (!std::filesystem::exists(filename))
					throw px::Exception(px::ErrorID::FileNotFound,
						{
							{ px::TagID::Filename, px::Str(filename_.string()) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});

				std::wifstream file(filename_.c_str());

				if (file)
				{
					px::String line;
					while (std::getline(file, line))
						lines_.emplace_back(line);
				}
				else
					throw px::Exception(px::ErrorID::FileRead,
						{
							{ px::TagID::Filename, px::Str(filename_.string()) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});
			}

			const px::String& operator[](unsigned int lineNumber) const
			{
				if (lineNumber < LineCount())
					return lines_[lineNumber];
				else
					throw px::Exception(px::ErrorID::FileLineInvalid,
						{
							{ px::TagID::Filename, px::Str(filename_.string()) },
							{ px::TagID::LineNumber, px::Str(std::to_string(lineNumber)) }
						});
			}

			px::String& operator[](unsigned int lineNumber)
			{
				if (lineNumber < LineCount())
					return lines_[lineNumber];
				else
					throw px::Exception(px::ErrorID::FileLineInvalid,
						{
							{ px::TagID::Filename, px::Str(filename_.string()) },
							{ px::TagID::LineNumber, px::Str(std::to_string(lineNumber)) }
						});
			}

			const std::vector<px::String>& GetLines() const { return lines_; }
			px::String GetText() const { return Join(lines_); }
			void SetLines(const std::vector<px::String>& lines) { lines_ = lines; }
			void SetText(const px::String& syntax) { lines_ = Split(syntax); }

			void Write() const
			{
				std::wofstream file(filename_.c_str());

				if (file)
					file << GetText();
				else
					throw px::Exception(px::ErrorID::FileNotFound,
						{
							{ px::TagID::Filename, px::Str(filename_.string()) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});
			}

			bool FileExists() const
			{
				if (std::filesystem::exists(filename_))
					return !std::filesystem::is_directory(filename_);

				return false;
			}

			unsigned int LineCount() const { return static_cast<unsigned int>(lines_.size()); }


		protected:
			std::vector<px::String> lines_;
		private:
			std::filesystem::path filename_;
		};

	}
}

#endif // PX_UTIL_FILEIO_HPP