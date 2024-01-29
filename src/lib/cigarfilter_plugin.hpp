//
// Created by guillaume on 10/10/22.
//
#ifndef CIGAR_FILTER_PLUGIN_API
#define CIGAR_FILTER_PLUGIN_API

#include <boost/config.hpp>
#include <string>
#include <vector>

/**
 * @brief A basic plugin class
 * @note virtual [...] = 0; defines the functions as purely virtual
 *      - This is how it must be done for the dynamic-linking to work properly
 *      - This also makes the compiler to interpret cigarfilter_plugin an abstract class
 *      - See <a hrefhttps://stackoverflow.com/questions/2652198/difference-between-a-virtual-function-and-a-pure-virtual-function>here</a>
 *
 * This means that each function of this part of cigarfilter_plugin API must be implemented by the end-user
 *
 */
class cigarfilter_plugin {
    public:
        /**
         * @brief A simple struct to store the informations parsed from a CIGAR string
         * @details The following links define the CIGAR string format
         *      - <a href="https://github.com/NBISweden/GAAS/blob/master/annotation/knowledge/cigar.md">https://github.com/NBISweden/GAAS/blob/master/annotation/knowledge/cigar.md</a>
         *      - <a href="https://samtools.github.io/hts-specs/SAMv1.pdf">https://samtools.github.io/hts-specs/SAMv1.pdf</a>
         *      - <a href="https://sites.google.com/site/bioinformaticsremarks/bioinfo/sam-bam-format/what-is-a-cigar/>https://sites.google.com/site/bioinformaticsremarks/bioinfo/sam-bam-format/what-is-a-cigar/</a>
         *
         * @warning All members are public (struct), therefore you can change them at will; but at your own risks.
         */
        struct CIGAR {
            size_t M = 0; /**< Number of Matched or mismatched nucleotides */
            size_t eq = 0; /**< Number of Matched nucleotides */
            size_t I = 0; /**< Number of Inserted nucleotides */
            size_t D = 0; /**< Number of Deleted nucleotides */
            size_t X = 0; /**< Number of Substituated nucleotides */
            size_t H = 0; /**< Number of Hard-clipped nucleotides */
            size_t S = 0; /**< Number of Soft-clipped nucleotides */
            size_t P = 0; /**< Number of Padded nucleotides */
            size_t N = 0; /**< Number of Skipped (or "Spliced") nucleotides
 *
 *       In cDNA-to-genome alignment, we may want to distinguish introns from deletions in exons.
 *       We introduce openation 'N' to represent long skip on the reference sequence. Suppose the spliced alignment is:

                REF:AGCTAGCATCGTGTCGCCCGTCTAGCATACGCATGATCGACTGTCAGCTAGTCAGACTAGTC

                Read:         GTGTAACCC................................TCAGAATA

                where '...' on the read sequence indicates intron. The CIGAR for this alignment is : 9M32N8M. */

            bool empty = false; /**< Set to true if the CIGAR string is "*" */
            bool error = false; /**< @warning This catches only a very limited subset of errors. When true, the CIGAR is invalid.
                                  * It is set to true if at least one group is not in {"X","M","N","P","H","S","=","D","I"} */

            std::vector<std::tuple<char,size_t>> groups; /**< Each tuple is made of (group_type,group_size) */

            std::string as_string = ""; /**< The original CIGAR string. */
        };

        virtual std::string name() const = 0; /**< Returns the name of the plugin (useless) */

        /**
         * @brief The user-defined filter. Returns true if the line passes the filter, false otherwise
         * @param parsed_cigar The CIGAR parsed by @ref cf::parse_CIGAR(
         */
        virtual bool filter(CIGAR &parsed_cigar,std::string &samline) = 0;

        virtual ~cigarfilter_plugin() {}
};

/** @brief A macro that simplifies the boilerplate code to write a filter function (can work only once per project) */
#define CIGARFILTER_PLUGIN(...)                                                       \
namespace plugins_ns {                                                                  \
    class default_filter : public cigarfilter_plugin {                                  \
        public:                                                                         \
            default_filter() {                                                          \
                                                                                        \
            }                                                                           \
                                                                                        \
            std::string name() const {                                                  \
                return "default_filter";                                                \
            }                                                                           \
                                                                                        \
        __VA_ARGS__  /* the user-defined "filter" function will be inserted here */     \
                                                                                        \
        ~default_filter() {                                                             \
                                                                                        \
        }                                                                               \
    };                                                                                  \
    extern "C" BOOST_SYMBOL_EXPORT default_filter plugin;                               \
    default_filter plugin;                                                              \
}

#endif
