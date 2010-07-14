package org.omras2;
import lombok.Getter;
import lombok.Setter;

public class Status
{
	@Getter @Setter private int numFiles;
	@Getter @Setter private int dims;
	@Getter @Setter private int dudCount;
	@Getter @Setter private int nullCount;
	@Getter @Setter private int length;
	@Getter @Setter private int dataRegionSize;
	@Getter @Setter private boolean isL2Normed;
	@Getter @Setter private boolean hasPower;
	@Getter @Setter private boolean hasTimes;
	@Getter @Setter private boolean hasRefs;
}
