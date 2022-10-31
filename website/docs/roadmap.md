---
sidebar_position: 2
---

# Roadmap

This is the short term roadmap for the new .NET Core version of illNES.

Progress is being tracked and managed by issues and projects, but milestones are informed by this document.

Because this is short term it basically just roadmaps the CPU development for now. A complete CPU takes us beyond the C++ version anyway, so as that goal looms, we'll reconsider the roadmap for the rest of the project.

This is also a fairly brief overview, as Issue Milestones and Projects provide better detailed information on progress (milestones) and order / state (Projects).

# CPU

All CPU work is managed in the `MOS6502` Project.

Milestones are listed below (and sometimes grouped)

- [x] Barebones CPU
- [x] Addressing Modes
- [ ] Instruction set
  - [x] Implied Mode only
  - [ ] Memory / Register ops
  - [ ] Bitwise / Math ops
  - [ ] Branches
  - [ ] Jumps
- [ ] C++ Feature Parity

C++ feature parity will essentially consist of writing a custom computer that uses the CPU, such that programs can be run for testing behaviour. There will be some throwaway work, but also some that will inform future development (e.g. display)