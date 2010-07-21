import junit.framework.*;
import java.io.File;
import org.omras2.*;

public class TestInsert extends TestCase
{
	File testDBFile;
	File testFeatureFile;
	protected void setUp()
	{
		testDBFile = new File("testfiles/test.adb");
		testFeatureFile = new File("testfiles/testfeature");
		if(testDBFile.exists())
			testDBFile.delete();
	}

	public void testInsertFile()
	{
		AudioDB testDB = new AudioDB(testDBFile);
		assertTrue("DB created", testDB.create(1, 1, 1));
		testDB.open(AudioDB.Mode.O_RDWR);
		assertTrue("Insert feature file", testDB.insert(testFeatureFile));
		Status status = testDB.getStatus();
		assertEquals("One feature in db", 1, status.getNumFiles());
		assertEquals("1D Feature", 1, status.getDim());
		System.out.println(status.hasReferences());
		System.out.println(status.isL2Normed());
		System.out.println(status.hasPower());
		System.out.println(status.hasTimes());
	}

	public void testInsertFileReadOnly()
	{
		AudioDB testDB = new AudioDB(testDBFile);
		assertTrue("DB created", testDB.create(1, 1, 1));
		testDB.open(AudioDB.Mode.O_RDONLY);
		assertFalse("Insert feature file", testDB.insert(testFeatureFile));
	}
}
