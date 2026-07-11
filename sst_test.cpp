// Headless SingleStepTests (sm83) harness. Reuses the emulator's CPU by including
// cpu.cpp with SST_TEST defined (flat 64KB memory, no SDL, no IO side effects).
// Reads a compact test stream on stdin (produced by run_sst.py) and reports results.
#define SST_TEST
#include "cpu.cpp"

int main()
{
    memory = new uint8_t[0x10000]();

    long total = 0, passed = 0, cyc_fail = 0;
    int shown = 0;

    // Per test: initial(a f b c d e h l sp pc ime ie) n (addr val)*  final(same, no ie) n (addr val)*  ncycles
    long ia, if_, ib, ic, id, ie_, ih, il, isp, ipc, iime, iie;
    while (std::cin >> ia)
    {
        std::cin >> if_ >> ib >> ic >> id >> ie_ >> ih >> il >> isp >> ipc >> iime >> iie;
        int ninit;
        std::cin >> ninit;
        // Set initial state
        registers.a = ia; registers.f = if_; registers.b = ib; registers.c = ic;
        registers.d = id; registers.e = ie_; registers.h = ih; registers.l = il;
        registers.sp = isp; registers.pc = ipc;
        ime_flag = iime; ei_flag = false; ei = false; halted = false; stopped = false;
        memory[0xFFFF] = iie;
        for (int i = 0; i < ninit; i++) { long ad, v; std::cin >> ad >> v; memory[ad & 0xFFFF] = v; }

        m_cycles = 0;
        execute_instruction();

        // Read expected final
        long fa, ff, fb, fc, fd, fe, fh, fl, fsp, fpc, fime;
        std::cin >> fa >> ff >> fb >> fc >> fd >> fe >> fh >> fl >> fsp >> fpc >> fime;
        int nfinal; std::cin >> nfinal;

        bool ok = true;
        std::string why;
        auto chk = [&](const char *n, long got, long exp){ if (got != exp && ok){ ok=false; char b[64]; snprintf(b,sizeof b,"%s got=%02lX exp=%02lX",n,got,exp); why=b; } };
        chk("A", registers.a, fa);
        chk("F", registers.f & 0xF0, ff & 0xF0);
        chk("B", registers.b, fb); chk("C", registers.c, fc);
        chk("D", registers.d, fd); chk("E", registers.e, fe);
        chk("H", registers.h, fh); chk("L", registers.l, fl);
        chk("SP", registers.sp, fsp); chk("PC", registers.pc, fpc);
        for (int i = 0; i < nfinal; i++)
        {
            long ad, v; std::cin >> ad >> v;
            if (ok && memory[ad & 0xFFFF] != (v & 0xFF)) { ok = false; char b[64]; snprintf(b,sizeof b,"RAM[%04lX] got=%02X exp=%02lX", ad, memory[ad&0xFFFF], v&0xFF); why=b; }
        }
        long ncyc; std::cin >> ncyc;
        if (m_cycles != ncyc) cyc_fail++;

        total++;
        if (ok) passed++;
        else if (shown++ < 4) printf("  FAIL #%ld: %s\n", total, why.c_str());
    }

    printf("SUMMARY %ld/%ld passed  cycle_mismatches=%ld\n", passed, total, cyc_fail);
    return (passed == total) ? 0 : 1;
}
