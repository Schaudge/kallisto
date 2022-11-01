#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <unordered_map>
#include <set>

#include <CompactedDBG.hpp>
#include <BlockArray.hpp>

// Unitig to transcript
struct u2t {
    // Transcript ID
    uint32_t tr_id;
    // Position of unitig within transcript
    uint32_t pos;
    u2t() {};
    u2t(uint32_t tr_id, uint32_t pos) : tr_id(tr_id), pos(pos) {};
};

class Node: public CDBG_Data_t<Node> {
    public:
        uint32_t id;
        // Mosaic Equivalence Class:
        // Each kmer in the unitig can have a different equivalence class
        BlockArray<Roaring> ec;

        // Positing of unitig within each of its transcripts
        // Most significant bit stores the sense of the transcript w.r.t. the
        // unitig
        std::vector<uint32_t> pos;

    Node() : id(-1) {}

    // Returns [j, k), j<=i, k>=i, where j-1 is the last kmer to have a
    // different EC than kmer i, and k is the first kmer to have a different
    // EC than i.
    std::pair<size_t, size_t> get_mc_contig(size_t i) const {
        return ec.get_block_at(i);
    }

    void concat(const UnitigMap<Node>& um_dest, const UnitigMap<Node>& um_src) {
    }

    void merge(const UnitigMap<Node>& um_dest, const UnitigMap<Node>& um_src) {
    }

    void clear(const UnitigMap<Node>& um_dest) {
        ec.clear();
        pos.clear();
    }

    void extract(const UnitigMap<Node>& um_src, bool last_extraction) {
    }

    void serialize(std::ofstream& out) const {

        size_t tmp_size;

        // 1 Write id
        out.write((char *)&id, sizeof(id));

        // 2 Write mosaic equivalence class
        ec.serialize(out);

        // 3 Write the positions of each transcript
        tmp_size = pos.size();
        out.write((char *)&tmp_size, sizeof(tmp_size));
        for (uint32_t p : pos) {
            out.write((char *)&p, sizeof(p));
        }
    }

    void deserialize(std::ifstream& in) {

        size_t tmp_size;
        uint32_t tmp_uint;

        // 1 Read id
        in.read((char *)&id, sizeof(id));

        // 2 Read mosaic equivalence class
        ec.deserialize(in);

        // 3 Read the positions of each transcript
        in.read((char *)&tmp_size, sizeof(tmp_size));
        pos.reserve(tmp_size);
        for (size_t i = 0; i < tmp_size; ++i) {
            in.read((char *)&tmp_uint, sizeof(tmp_uint));
            pos.push_back(tmp_uint);
        }
    }
};

#endif
